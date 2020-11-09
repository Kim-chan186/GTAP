#pragma once

#ifndef POPULATION_H
#define POPULATION_H

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "utilcv/Debug.h"
#include "utilcv/help_mat.h"

#include "individuals.h"

using namespace cv;
using namespace std;


namespace GA {
	struct str_edge
	{
		str_edge(int Name1, int Name2, double Cost) : name1(Name1), name2(Name2), cost(Cost) {}
		str_edge() { name1 = -1; name2 = -1; cost = -1; }
		int name1; int name2;
		double cost;
	};
	bool operator > (str_edge& a, str_edge& b) {
		return a.cost > b.cost;
	}
	bool operator < (str_edge& a, str_edge& b) {
		return a.cost < b.cost;
	}

	class population {
	private:
		Mat* frame_Node, frame, cost;
		vector<Point2i> coordi; // node
		vector<individuals*> ind_s;
		string node_file_name;
		string wall_file_name;
		static uint generate;
		vector<vector<uint>> neighbor;
		cv::Size map_size;
	public:
		population(string population_number, vector<Point2i> coordi = vector<Point2i>(), cv::Size map_size = cv::Size(1000, 1000));
		~population();

		void findGraph(cv::Mat Mask, std::string nodeFile = "coordi1.txt");
		bool draw_all_node();
		void show();
		void save();
		bool load(bool wall_flag = 0);
		void _sort();
		bool nextGenerate();

		double canMoved(const cv::Point& n1, const cv::Point& n2, const cv::Mat& mask);
		bool fire(Mat& mask, Point p, bool flag = 0);
	}; // end class population
	uint population::generate = 0;

	void population::show() {
		if (!VISUALIZATION)
			return;

		//imshow("population", *this->frame_Node);
		imshow("population", this->frame);
	}

	void population::save() {
		ofstream fout;
		fout.open(this->node_file_name);

		for (auto& item : coordi) {
			fout << item.x << " " << item.y << endl;
		}
		fout.close();

		fout.open(this->wall_file_name);
		//
		fout.close();
	}

	// precondition file name 정하고 호출
	bool population::load(bool wall_flag) {
		string x, y;
		ifstream fin("log.txt");

		try {
			fin.open(this->node_file_name);
			if (!fin) {
				throw - 1;
			}
			fin >> x >> y;
			while (!fin.eof()) {
				Point Node(stoi(x), stoi(y));
				//cout << Node << endl;
				coordi.push_back(Node);
				fin >> x >> y;
			}
			fin.close();

			if (wall_flag) {
				fin.open(this->wall_file_name);
				if (!fin) {
					throw - 1;
				}
				fin.close();
			}
		}
		catch (int error_code) {
			cout << " ** error::population.h/class population/load() : is worng file name !! \n";
			return 0;
		}
		return 1;
	}


	bool population::draw_all_node() {
		if (!VISUALIZATION)
			return 0;
		else if (frame_Node->empty()) {
			cout << " ** error::population.h/class population/draw_all_node() : frame is empty, population isn't exist !! \n";
			return 0;
		}
		else if (coordi.empty()) {
			cout << " ** error::population.h/class population/draw_all_node() : coor is empty, not enough Node !! \n";
			return 0;
		}

		//for (auto iter1 = (++coordi.begin()); iter1 != coordi.end(); iter1++) {
		//	for (auto iter2 = coordi.begin(); iter2 != iter1; iter2++) {
		//		line(*frame, *iter1, *iter2, sc_line, line_thin);
		//	}
		//}


		for (auto& item : coordi) {
			// cout << item << endl;
			circle(*frame_Node, item, radius, sc_circle, -1); // radius and sc_circle is defined at help_population.h
		}


		return 1;
	}

	// 입력받은 노드 근방의 맵으로 조각내서 이동가능한지 판단하는 함수, Cost를 반환
	double population::canMoved(const cv::Point& n1, const cv::Point& n2, const cv::Mat& mask_) {
		Point2d p2d = n1 - n2;
		double dcost = sqrt(p2d.x * p2d.x + p2d.y * p2d.y);
		// 거리가 준내 멀면 삭제
		if (dcost > 500) {
			return MAXCOST;
		}

		Point p1(min(n1.x, n2.x), min(n1.y, n2.y)), p2(max(n1.x, n2.x), max(n1.y, n2.y));
		Mat mask(mask_(cv::Rect(p1, p2)).clone());		// const shallow copy

		queue<Point> fire; Point* cuser;
		fire.push(n1);
		int x, y;
		
		while (!fire.empty()) {
			cuser = &fire.front();
			// 리소스를 줄이기 위해서는 대각선 이동이 가능한 것과 불가능한 것을 구분
			if (mask.at<uchar>(cuser->y, cuser->x)) {
				x = cuser->x; y = cuser->y;
				mask.at<uchar>(y,x) = 0;
				fire.push(Point(x+1, y));
				fire.push(Point(x,	 y+1));
				fire.push(Point(x-1, y));
				fire.push(Point(x,	 y-1));
			}

			fire.pop();
		}
	}

	//flag = 1 경우 대각선 성분을 포함함
	bool population::fire(Mat& mask, Point p, bool flag) {
		return 0;
	}

	// Binary Map and Node를 입력받아 graph 생성
	void population::findGraph(const cv::Mat Mask, std::string nodeFile) {
		using namespace std;
		using namespace cv;

		int end = this->coordi.size();
		double dcost;

		vector<str_edge> costlist;
		this->cost = Mat::eye(end, end, CV_64FC1) * MAXCOST;
		Point p2d;
		costlist.reserve(end * end - end);
		this->neighbor.reserve(end);

		for (int i = 0; i < end; i++) {
			vector<uint> vec;
			this->neighbor.emplace_back(vec);
			for (int j = i + 1; j < end; j++) {
				p2d = this->coordi.at(i) - this->coordi.at(j);
				dcost = sqrt(p2d.x * p2d.x + p2d.y * p2d.y);
				//dcost = this->canMoved(this->coordi.at(i), this->coordi.at(j), Mask);

				this->cost.at<double>(i, j) = dcost;
				this->cost.at<double>(j, i) = dcost;
				costlist.emplace_back(str_edge(i, j, dcost));
			}
		}
		sort(costlist.begin(), costlist.end());
		vector<str_edge>::iterator iter = costlist.begin();
		int i = costlist.size() / 3;
		int n1, n2;
		for (; i; i--, iter++) {
			n1 = (*iter).name1;
			n2 = (*iter).name2;
			neighbor.at(n1).emplace_back(n2);
			neighbor.at(n2).emplace_back(n1);
		}
	}

	// load population 미구현
	// AUTOSET keyworld를 사용하기 위해서는 lin318. txt, .png파일이 정상적으로 존재해야함
	// 융합과학 기술원을 기준으로 1을 사용하여 '벽을 고려하지 않는' 노드를 생성함 > 벽 부분은 추후 생성
	population::population(string population_number, vector<Point2i> coordi, cv::Size map_size) {

		std::srand((unsigned int)time(NULL));


		// 가장 최근 파일로 하도록 수정
		if (population_number == AUTOSET) {
			this->node_file_name = "lin318small.txt";
			//this->node_file_name = "coordi" + to_string(population_number) + ".txt";
			this->wall_file_name = "wall_lin318small.txt";
			Mat frame = imread("lin318small.png");
			this->map_size = frame.size();

			this->node_file_name = "lin318.txt";
			this->map_size = cv::Size(3100, 4200);
		}
		else {
			this->coordi = coordi;
			this->node_file_name = "coordi" + population_number + ".txt";
			this->wall_file_name = "wall" + population_number + ".txt";
			this->map_size = map_size;
		}
		this->load();

		cout << "num of node : " << this->coordi.size() << endl;
		this->findGraph(Mat(), this->node_file_name);		
		//this->findGraph(getBinaryMap(), this->node_file_name);		// 원래 map을 입력 받아서 하는 것으로 하려 했음나 필요없어져서 만들지 않음

		int end = this->coordi.size() * 4;
		if (!end) { // == 0
			cout << " * population.h error : population : coordi is empty \n";
			return;
		}
		ind_s.reserve(end);
		individuals* x = &individuals();
		x->setMapSize(this->map_size);		TTT
		for (int i = 0; i < end; i++) {
			//cout << i << "th enforcement \n";
			 x = new individuals(this->cost, this->coordi);
			//debug::print_pointer(x->genome, 5);
			//x->genome = nullptr;
			this->ind_s.emplace_back(x);
		}
		this->_sort();		TTT
		if (VISUALIZATION) {
			//this->frame_Node = new Mat(map_size, CV_8UC3, Scalar::all(0));
			//this->frame = new Mat(map_size, CV_8UC3, Scalar::all(0));
			//this->draw_all_node();

			namedWindow("population", WINDOW_AUTOSIZE);
			moveWindow("population", -1050, 0);

			//this->show();
		}
	}// end population Constructor


	// 순위별로 이미지 생성
	void population::_sort() {
		//for (auto& item : ind_s) {
		//	cout << item.cost() << endl;
		//	//debug::print_pointer(item.genome, 5);
		//}

		vector<individuals*>::iterator iter = ind_s.begin();
		sort(ind_s.begin(), ind_s.end(), sort_func); //오름차순  less<individuals>()

		if (VISUALIZATION) {
			printf("\t\t");
			//for (int i = 0; i < 5; i++) {
			//	Mat buf = *(*(iter++))->img(this->coordi);
			//	for (int j = 0; j < 4; j++) {
			//		if(i==0) printf(" %.3f, ", (*iter)->cost());
			//		hconcat(buf, *(*(iter++))->img(this->coordi), buf);
			//	}
			//	// 얉은 복사로 수정하기
			//	if (i == 0) {
			//		this->frame = buf.clone();
			//		//nn;
			//	}
			//	else
			//		vconcat(this->frame, buf, this->frame);
			//}
			this->frame = *(*(iter))->img(this->coordi);
			printf(" %.3f, ", (*iter++)->cost());
		}
		printf(" **  **  **  ** \x1b[36m%d Generate \x1b[0m **  **  **  ** \n\n", ++this->generate);
		LK = generate/10 + LIN;
	}// // 


	bool population::nextGenerate() {
		// random 생성
		int end = coordi.size() * 4; // all
		int start = end * LGROUP;
		individuals* x;
		for (int i = start; i < end; i++) {
			x = ind_s.at(i);
			x->release();
			delete x;
			ind_s.at(i) = new individuals(this->cost, this->coordi);
			if (ind_s.at(i)->check_cycle()) 				quick_exit(EXIT_SUCCESS);
		} cout << "end next random individuals \n";

		// 교배
		start = end * SGROUP;
		int cros = start;
		for (int i = start; i < end; i++) {
			ind_s.at(i)->NCX(this->cost, this->coordi, this->ind_s.at(rand() % cros), neighbor);
			//ind_s.at(i)->SCX(this->cost, this->coordi, this->ind_s.at(rand() % cros));

			if (ind_s.at(i)->check_cycle())					quick_exit(EXIT_SUCCESS);
		} cout << "end next NCX individuals \n";
		this->_sort();
		this->show();
	}

	population::~population() {
		cout << "전체 소멸 \n\n";
		delete this->frame_Node, this->frame, this->cost;

		for (auto& item : ind_s) {
			delete& item;
		}
	}
}// End chan


#endif /// !EMOTION_H