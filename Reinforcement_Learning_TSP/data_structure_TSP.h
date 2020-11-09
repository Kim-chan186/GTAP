#pragma once

#ifndef DATA_TSP_H
#define DATA_TSP_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


#include "utilcv/Debug.h"
#include "utilcv/help_mat.h"
#include "Genetic.h"



using namespace std;


namespace TSP {


	cv::Mat* Map_frame = nullptr;

	cv::Mat* Map_line = nullptr;

	cv::Mat* Map_erase = nullptr;

	cv::Mat* Map_font = nullptr;

	cv::Mat* costs = nullptr;




	cv::Size* Map_size = nullptr;

	int num_obstacle = -1;

	cv::Point* plist = nullptr;



	// xy to digree 
	// atan(1.732) * 180.0 / 3.14 = 60
	const double xy2d = 180.0 / 3.141529;

	const double thW = (3.141529 / 2 * 10 / 90)*2;



	//font1 작은맵용(35)
	/*float font_scale = 0.5;
	int font_thickness = 2,
		radius = 15;
	Point shift1(-5, 5),  shift2(-11, 5);*/

	//font2 큰맵용(100)
	/*float font_scale = 0.4;
	int font_thickness = 2,
		radius = 10;
	Point shift1(-4, 4), shift2(-8, 4);*/

	//font3 더큰맵용(200)
	float font_scale = 0.3;
	int font_thickness = 1,
		radius = 10;
	Point shift1(-4, 4), shift2(-8, 4);


	int path_Boruvka[100] = {};
	



	// 최하단 전역 class 선언 참고



	class Link {

	public:
		int i = -1;
		int j = -1;
		double cost = -1;

		Link(int _i, int _j, double _cost)
			: i(_i), j(_j), cost(_cost) {}

		bool operator<(Link n) const {
			return this->cost < n.cost;
		}

	};// end Link




	class  Node {

	public:
		int num = -1;


		Node* next = nullptr;			//end 방향
		Node* previous = nullptr;		//head 방향

		Node() {};
		Node(int _num) : num(_num) {}


		friend void print_link_ones(Node* np);


		bool operator<<(Node& node) {
			//연결가능하고, 입력 노드가 반대쪽 끝이면 안됨
			if (this->next == nullptr
				& this->get_head() != &node)
			{
				if (node.previous == nullptr)
				{
					node.previous = this;
					this->next = &node;
				}
				//뒤집기
				else if (node.next == nullptr)
				{
					node.reverse();
					node.previous = this;
					this->next = &node;
				}
				else
					return false;
			}

			else if (this->previous == nullptr
				& this->get_end() != &node)
			{
				if (node.next == nullptr)
				{
					node.next = this;
					this->previous = &node;
				}
				//뒤집기
				else if (node.previous == nullptr)
				{
					node.reverse();
					node.next = this;
					this->previous = &node;
				}
				else
					return false;
			}

			else
				return false;

			return true;

		}//end operator<<



		//head와 end를 바꿈, head나 end 에서만 수행
		bool reverse() {

			//when end
			if (this->next == nullptr) {
				Node* ep = this;

				while (ep != nullptr) {
					Node* buf = ep->previous;

					ep->previous = ep->next;
					ep->next = buf;

					///print_link_ones(ep);

					//뒤입어서 반대로
					ep = ep->next;
				}

				return true;
			}

			//when head
			else if (this->previous == nullptr) {
				Node* hp = this;

				while (hp != nullptr) {
					Node* buf = hp->previous;

					hp->previous = hp->next;
					hp->next = buf;

					///print_link_ones(hp);

					//뒤입어서 반대로
					hp = hp->previous;
				}

				return true;
			}

			return false;

		}// end reverse



		//수정 :: 공유하는 숫자로 자기가 포함된 길이를 알면 좋겠다
		Node* get_end() {
			Node* ep = this;

			while (ep->next != nullptr) {
				ep = ep->next;
			}
			return ep;
		}



		Node* get_head() {
			Node* hp = this;

			while (hp->previous != nullptr) {
				hp = hp->previous;
			}
			return hp;
		}


		void print_link() {
			Node* np = get_head();
			cout << "< head >" << " - ";

			while (np != nullptr) {
				cout << "<" << np->num << "> - ";
				np = np->next;
			}
			
			cout << "< end >" << endl;
		}


	};//end Node


	void print_link_ones(Node* np) {

		if (np->next == nullptr) {// 1

			if (np->previous == nullptr) {
				//연결이 없는 노드
				cout << "< head >" << " - <" << np->num << "> - " << "< end >" << endl;
			}
			else
				cout << np->previous->num << " - <" << np->num << "> - " << "< end >" << endl;
		}
		else if (np->next->num == num_obstacle) {// 1

			if (np->previous == nullptr) {
				cout << "무언가 이상";
			}
			else
				cout << np->previous->num << " - <" << np->num << "> - " << "< End >" << endl;
		}

		else if (np->previous == nullptr) {// 2
			cout << "< head >" << " - <" << np->num << "> - " << np->next->num << endl;
		}
		else if (np->previous->num == num_obstacle) {// 2
			cout << "< Head >" << " - <" << np->num << "> - " << np->next->num << endl;
		}

		else// 3
			cout << np->previous->num << " - <" << np->num << "> - " << np->next->num << endl;

	}



	// 최상단 전역 value 선언 참고

	Node* st = nullptr;

	Node* Nhead = nullptr;



	double ReLU(double b) {
		if (b < 0) {
			b = 0;
		}
		return b;
	}



	double sigmoid(double x) {
		return 800 / (1 + exp(-x));
	}



	// 최소 회전반경
	// l / tan(theta) + d/2
	// l : 차축 사이거리 5r(3)/12 = 1.01036, d : 차축 길이 0.5, 최대 차축 각 : 30
	// 10 pixel : 1m
	const double rds = 20.0; //


	// 노드 ijk 가 이루는 각중 작은 각
	// i에서 j로 이동 후 k로 이동하기 위해 움직여야 하는 각도
	double rotation(int i, int j, int k) {

		double a = costs->at<Vec2d>(i, j)[1];  // i to j
		double b = costs->at<Vec2d>(j, k)[1];  // j to k

		double abs_ba = abs(b - a);

		double cost = abs_ba - ReLU(2*abs_ba - 360); //theta

		//printf("          (%d, %d, %d) : %.3f  >  %.3f\n", i, j, k, cost, cost * thW * 10 + sigmoid(cost));
		//printf("/ %.3f /", cost);

		double m = tan(cost / xy2d);
		double sm = sqrt(m*m + 1);
		double s = sqrt(2 * m*m - 2 * m*sm + 3);

		//변수 재이용
		a = (1 - m * sm + s) * rds / (m * m + 1);
		b = m * a + rds * sm;

		//sm*x*x + - 2 * (a + m * b) * x + a*a + b*b - r*r = 0
		//근의 공식(중근)
		//double x = (a + m * b) / (m*m + 1);

		// y = tan()x, d = r(x^2 + y^2)
		double d = (a + m * b) / sm;

		double cost2 = sigmoid(d - costs->at<Vec2d>(j, k)[0]);

		if (cost2 > 10) {
			printf("\n %d, %d, %d,  : %.3f  //(%.0f > %.0f) m = %.3f, theta = %.3f \n",
				i, j, k, cost2, d, costs->at<Vec2d>(j, k)[0], m, cost / xy2d);
			printf("a : %.3f, b : %.3f,  s : %.3f \n", a, b, s);
		}
			

		//위 식의 값(x)보다 멀리 있어야 이동 가능
		return cost * thW*10;// +cost2;
	}



	double rotation(int i, int j, int k, int m) {

		//printf("          (%d, %d, %d, %d) : ", i, j, k, m);

		double cost = rotation(i, j, k) + rotation(j, k, m);

		//printf("  > %.3f \n", cost);

		//waitKey(0);

		return cost;
	}



	//algorithm의 sort에 사용
	bool sort_LinkNode(Link n1, Link n2) {
		return n1.cost < n2.cost;
	}




	//원형 노드용
	double get_cost() {
		int previous_num = Nhead->num;
		Node* np = Nhead->next;
		double sum = 0;
		double sum_r = 0;
		printf("<%2d>", Nhead->num);

		while (true) {
			//printf(" - %2d : %.3f \n", np->num, costs->at<cv::Vec2d>(previous_num, np->num)[0]);
			printf(" - %2d", np->num);
			sum_r += rotation(np->previous->num, np->num, np->next->num);
			sum += costs->at<cv::Vec2d>(previous_num, np->num)[0];
			previous_num = np->num;

			if (np == Nhead)
				break;

			np = np->next;
		}
		//printf(">\n\n", np->num);
		cout << "\n cost : " << sum << " + " << sum_r << endl;
		return sum;
	};

}// End chan


#endif /// !EMOTION_H