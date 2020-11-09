#pragma once

#ifndef INDIVIDUALS_H
#define INDICIDUALS_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "utilcv/Debug.h"
#include "utilcv/help_mat.h"


using namespace cv;
using namespace std;


namespace GA {
	struct str_genome
	{
		str_genome(int Name, double Cost) : name(Name), cost(Cost) {}
		str_genome() { name = -1; cost = -1; }
		int name;
		double cost;
		//int random;
	};
	bool operator > (str_genome& a, str_genome& b) {
		return a.cost > b.cost;
	}
	bool operator < (str_genome& a, str_genome& b) {
		return a.cost < b.cost;
	}

	class individuals
	{
	public:
		individuals() {};
		individuals(const Mat& costMat, const vector<Point2i>& vec);
		//individuals(individuals& ind_);	//copy generator
		bool SCX(const Mat& costMat, const vector<Point2i>& vec, const individuals* parents);
		bool NCX(const Mat& costMat, const vector<Point2i>& vec, const individuals* parents, const vector<vector<uint>>& neighbor);
		~individuals();
		void release();

		const Mat* img(const vector<Point2i>& vec); // when non - visuable mode, don't call this, 1/5 size population frame
		double cost() { return this->Cost; }
		bool operator >(const individuals& ind2) const { return this->Cost > ind2.Cost; }
		bool operator <(const individuals& ind2) const { return this->Cost < ind2.Cost; }
		bool check_cycle();
		void setMapSize(cv::Size s) { this->map_size = s; }
		//friend bool operator >(const individuals& ind1, const individuals& ind2);
		//friend bool operator <(const individuals& ind1, const individuals& ind2);

	private:
		int lengh;
		int* genome;
		int* genome2;
		Mat* frame, *small_frame;
		double Cost;

	protected:
		uint rand_genome(const bool* ptr) const;
		static cv::Size map_size;
		//오름차순
	};
	cv::Size individuals::map_size = cv::Size();

	bool sort_func(individuals* i1, individuals* i2) { return i1->cost() < i2->cost(); }
	//bool operator >(const individuals& ind1, const individuals& ind2) { return ind1.Cost > ind2.Cost; }
	//bool operator <(const individuals& ind1, const individuals& ind2) { return ind1.Cost < ind2.Cost; }
	 
	// random, init, precondition : call this->setMapSize(), after this generator
	individuals::individuals(const Mat& costMat, const vector<Point2i>& vec)
	{
		this->Cost = 0;
		this->frame = nullptr;
		this->small_frame = nullptr;
		this->lengh = vec.size();

		int* box = new int[this->lengh];
		int* genome = new int[this->lengh];
		int num;
		for (int cnt = 0; cnt < this->lengh; cnt++) {
			box[cnt] = cnt;
		}
		for (int cnt = 0; cnt < this->lengh; cnt++) {
			num = rand() % (this->lengh -cnt);
			genome[cnt] = box[num];
			box[num] = box[this->lengh - cnt - 1];
			box[this->lengh - cnt - 1] = genome[cnt];
		}
		
		Point2d p2d;
		for (int i = 0; i < this->lengh - 1; i++) {
			Cost += costMat.at<double>(genome[i], genome[i+1]);
		}

		if (VISUALIZATION) {
			//this->frame = new Mat(this->map_size, CV_8UC3, Scalar::all(0));
			//this->small_frame = new Mat(this->map_size /5, CV_8UC3, Scalar::all(0));	// necessary
			//for (int i = 0; i < this->lengh -1; i++) {
			//	line(*frame, vec.at(genome[i]), vec.at(genome[i + 1]), sc_edge, edge_thin);
			//}
			//line(*frame, vec.at(genome[0]), vec.at(genome[lengh - 1]), sc_edge, edge_thin);
			//resize(*frame, *small_frame, frame->size()/5);
		}
		this->genome2 = genome;
		this->genome = nullptr;
		delete[] box;
		//debug::print_pointer(this->genome, Node_count);
	}
	const Mat* individuals::img(const vector<Point2i>& vec) {

		if (VISUALIZATION) {
			if(nullptr != this->frame) delete this->frame;
			int d = 10;
			this->frame = new Mat(this->map_size/d, CV_8UC3, Scalar::all(0));
			for (int i = 0; i < this->lengh - 1; i++) {
				line(*frame, vec.at(genome2[i]) / d, vec.at(genome2[i + 1]) / d, sc_edge, edge_thin/3);
			}
			line(*frame, vec.at(genome2[0]) / d, vec.at(genome2[lengh - 1]) / d, sc_edge, edge_thin/3);
			//resize(*frame, *small_frame, frame->size() / 10);
			//return this->small_frame;
			return this->frame;
		}
		else
			return &Mat();
	}

	// ptr의 값이 1인 것중 랜덤한 것을 반환
	uint individuals::rand_genome(const bool* ptr) const{
		int num = rand() % this->lengh;
		for (int i = this->lengh; i; i--, num++) {
			if (ptr[num % this->lengh])
				return num % this->lengh;
		}
		cout << " individuals.h error : rand_genome() : not enough node(genome) \n";
		return 0;
	}

	// next generate
	bool individuals::NCX(const Mat& costMat, const vector<Point2i>& vec, const individuals* parents, const  vector <vector<uint>>& neighbor)
	{
		int box_size = vec.size();
		vector<vector<str_genome>> genome_box; // linked list

		for (int i = 0; i < box_size; i++) {
			vector<str_genome> vec;
			genome_box.emplace_back(vec);
		}

		//debug::print_pointer(genome2, box_size);
		for (int i = 0; i < box_size; i++) {
			int N1 = parents->genome2[i], N2 = this->genome2[i],
				edge1 = i - 1 + (i == 0) * box_size,
				edge2 = (i + 1) * (i != (box_size - 1));
			//printf("%d (%d// %d, %d), (%d// %d %d) \n", i, N1, parents.genome2[edge1], parents.genome2[edge2], N2, this->genome2[edge1], this->genome2[edge2]);
			//genome_box.at(N1).emplace_back(str_genome(parents->genome2[edge1], costMat.at<double>(parents->genome2[edge1], N1) * (rand() % 10) / 10));
			//genome_box.at(N1).emplace_back(str_genome(parents->genome2[edge2], costMat.at<double>(parents->genome2[edge2], N1) * (rand() % 10) / 10));
			//genome_box.at(N2).emplace_back(str_genome(this->genome2[edge1], costMat.at<double>(this->genome2[edge1], N2) * (rand() % 10) / 10));
			//genome_box.at(N2).emplace_back(str_genome(this->genome2[edge2], costMat.at<double>(this->genome2[edge2], N2) * (rand() % 10) / 10));
			genome_box.at(N1).emplace_back(str_genome(parents->genome2[edge1], costMat.at<double>(parents->genome2[edge1], N1) + (rand() % 200)));
			genome_box.at(N1).emplace_back(str_genome(parents->genome2[edge2], costMat.at<double>(parents->genome2[edge2], N1) + (rand() % 200)));
			genome_box.at(N2).emplace_back(str_genome(this->genome2[edge1], costMat.at<double>(this->genome2[edge1], N2) + (rand() % 200)));
			genome_box.at(N2).emplace_back(str_genome(this->genome2[edge2], costMat.at<double>(this->genome2[edge2], N2) + (rand() % 200)));
		}
		bool* genome_check = new bool[box_size];
		for (int i = 0; i < box_size; i++) {
			sort(genome_box.at(i).begin(), genome_box.at(i).end());
			genome_check[i] = true;
		}

		int a, b, node = rand() % box_size, name, j;
		genome_check[node] = false;	// start node check
		int* new_genome = new int[box_size];
		str_genome v1, v2, v3, v4;
		for (int i = 0; i < box_size - 1; i++) {
			//cout << node << " @ ";
			//debug::print_pointer(genome_check, box_size);
			//cout << "celect : " << node << endl;
			new_genome[i] = node;
			for (j = 0; j < 4; j++) {
				name = genome_box.at(node).at(j).name;
				if (genome_check[name]) {
					genome_check[name] = false;
					break;
				}
			}
			if (j > 3) {
				//debug::print_pointer(genome_check, this->lengh, "genome_check");
				name = rand_genome(genome_check);
				genome_check[name] = false;
			}
			node = name;
		}
		new_genome[box_size - 1] = node;

		delete[] genome_check;
		delete[] this->genome2;
		//debug::print_pointer(new_genome, this->lengh);
		// DNA mutation
		if ((rand() % 100)<LK) {
			int* DNA = new int[box_size + 2];	// circle형태를 쉽게 구성하기 위해서 -1 +1 한 경우만 계산, 사이즈가 2큰 배열을 생성
			copy(new_genome, new_genome + box_size, DNA + 1);
			DNA[0] = new_genome[box_size - 1];
			DNA[box_size + 1] = new_genome[0];

			//rel은 DNA를 기준으로 선택된 바꿀 부분, rel이 방출되어 i-1과 i사이에 들어가게 됨
			int rel = (rand() % box_size) +1; // 이동할 DNA 순서 DNA기준 1부터 시작 > DNA는 2사이즈 큰 배열
			int rel_node = DNA[rel];
			int rel_node1 = DNA[rel-1];
			int rel_node2 = DNA[rel+1];
			// _는 공통부분
			double before, before_ = costMat.at<double>(rel_node, rel_node1) + costMat.at<double>(rel_node, rel_node2);
			double after, after_ = costMat.at<double>(rel_node1, rel_node2);
			
			// i는 +2 size인 DNA기준
			for (int i = 1; i < box_size+2; i++) {
				if (rel == i)
					continue;
				before = before_ + costMat.at<double>(DNA[i - 1], DNA[i]);
				after = after_ + costMat.at<double>(DNA[i - 1], rel_node) + costMat.at<double>(rel_node2, DNA[i]);

				if (before > after) {
					//debug::print_pointer(new_genome, this->lengh);
					rel--;//new_genome 기준으로 변환
					if (i == 0) { i = box_size; } // 넘치는거 맞음
					else if (i == box_size + 1) { i = 0; }
					else { i--; }

					//cout << "[rel] : [" << rel << "] \t [i] : [" << i << "] \n";
					//cout << " rel  : [" << new_genome[rel] << "] \t  i  : [" << new_genome[i] << "] \n";

					if (rel > i) {
						//copy(new_genome + rel, new_genome + i, new_genome+rel+1); // i-1까지
						copy(new_genome + i, new_genome + rel, new_genome + i+1); //
						new_genome[i] = rel_node;	// i번째
					}
					else {
						//copy(new_genome + i+1, new_genome + box_size, new_genome+i+2);// i번
						copy(new_genome + rel + 1, new_genome + i, new_genome + rel);
						new_genome[i-1] = rel_node;	// i번째
					}

					break;
				}
			}
		}
		//debug::print_pointer(new_genome, this->lengh);
		this->genome2 = new_genome;
		this->Cost = 0;
		for (int i = 0; i < this->lengh - 1; i++) {
			this->Cost += costMat.at<double>(genome2[i], genome2[i + 1]);
		}
		this->Cost += costMat.at<double>(genome2[0], genome2[lengh - 1]);
		return 1;
	}

	// next generate
	bool individuals::SCX(const Mat& costMat, const vector<Point2i>& vec, const individuals* parents)
	{
		int box_size = vec.size();
		vector<vector<str_genome>> genome_box; // linked list

		for (int i = 0; i < box_size; i++) {
			vector<str_genome> vec;
			genome_box.emplace_back(vec);
		}

		//debug::print_pointer(genome2, box_size);
		for (int i = 0; i < box_size; i++) {
			int N1 = parents->genome2[i], N2 = this->genome2[i],
				edge1 = i - 1 + (i == 0) * box_size,
				edge2 = (i + 1) * (i != (box_size - 1));
			//printf("%d (%d// %d, %d), (%d// %d %d) \n", i, N1, parents.genome2[edge1], parents.genome2[edge2], N2, this->genome2[edge1], this->genome2[edge2]);
			genome_box.at(N1).emplace_back(str_genome(parents->genome2[edge1], costMat.at<double>(parents->genome2[edge1], N1)));
			genome_box.at(N1).emplace_back(str_genome(parents->genome2[edge2], costMat.at<double>(parents->genome2[edge2], N1)));
			genome_box.at(N2).emplace_back(str_genome(this->genome2[edge1], costMat.at<double>(this->genome2[edge1], N2)));
			genome_box.at(N2).emplace_back(str_genome(this->genome2[edge2], costMat.at<double>(this->genome2[edge2], N2)));
		}
		bool* genome_check = new bool[box_size];
		for (int i = 0; i < box_size; i++) {
			sort(genome_box.at(i).begin(), genome_box.at(i).end()); // -> 수정
			genome_check[i] = true;
		}

		int a, b, node = rand() % box_size, name, j, k;
		genome_check[node] = false;	// start node check
		int* new_genome = new int[box_size];
		str_genome v1, v2, v3, v4;
		
		for (int i = 0; i < box_size-1; i++) {
			//cout << node << " @ ";
			//debug::print_pointer(genome_check, box_size);
			//cout << "celect : " << node << endl;
			new_genome[i] = node;
			for (j = 0; j < 4; j++) {
				name = genome_box.at(node).at(j).name;
				if (genome_check[name]) {
					genome_check[name] = false;
					break;
				}
			}
			if (j > 3) {
				name = rand_genome(genome_check);
				genome_check[name] = false;

				//for (k = box_size - 1; k > -1; k--) {
				//	if (genome_check[k]) {
				//		genome_check[k] = false;
				//		name = k;
				//		break;
				//	}
				//}
			}
			node = name;
		}
		new_genome[box_size - 1] = node;
		delete[] genome_check;
		delete[] this->genome2;
		this->genome2 = new_genome;
		//debug::print_pointer(this->genome2, this->lengh);
		this->Cost = 0;
		for (int i = 0; i < this->lengh - 1; i++) {
			this->Cost += costMat.at<double>(genome2[i], genome2[i + 1]);
		}
		this->Cost += costMat.at<double>(genome2[0], genome2[lengh - 1]);
		return 1;
	}

	// 2개 존재하는 Node를 출력
	bool individuals::check_cycle() {
		vector<int> vec;
		for (int i = 0; i < this->lengh; i++) {
			vec.emplace_back(this->genome2[i]);
		}
		sort(vec.begin(), vec.end());
		//debug::print_vec(vec);
		int before = vec.at(0);
		for (int i = 1; i < this->lengh; i++) {
			if (vec.at(i) == before) {
				cout << " ** individuals.h error : check_cycle() : made cycle at genome <" << before << "> Length = " << this->lengh << "\n";
				debug::print_pointer(this->genome2, this->lengh);
				return 1;
			}
			before = vec.at(i);
		}
		return 0;
	}

	//individuals::individuals(individuals& ind_) {
	//	this->lengh = ind_.lengh;
	//	this->Cost = ind_.Cost;

	//	copy(ind_.genome2, ind_.genome2 + lengh, this->genome2);
	//	this->small_frame = small_frame;
	//}

	void individuals::release(){
		//cout << "소멸 ";
		delete[] genome2;
		delete frame;
		delete small_frame;
		//cout << "성공";
		//this->~individuals();
	}

	individuals::~individuals()
	{
		//cout << "소멸자 호출" << endl;
	}


}// End chan


#endif /// !EMOTION_H