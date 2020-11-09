#pragma once

#ifndef SHOW_TSP_H
#define SHOW_TSP_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <list>

#include "utilcv/Debug.h"
#include "utilcv/help_mat.h"

#include "data_structure_TSP.h"

using namespace cv;
using namespace std;


namespace TSP {

	list<Point2i> erase_list; //Queue


	void show(string str = "") {

		//if (str == "Boruvka") return;
		//if (str == "start1") return;

		if (str == "relaxed") return;
		if (str == "start2") return;
		if (str == "start3") return;
		if (str == "start4") return;

		if (str == "2-opt") return;
		if (str == "start5") return;

		imshow("TSP Map", *Map_frame + *Map_line + *Map_erase - *Map_font);
		waitKey(0);
	}

	





	void drow(int i) {
		if (i < 10){
			putText(*Map_font, to_string(i),
				plist[i] + shift1,
				FONT_HERSHEY_DUPLEX,			//FONT_HERSHEY_PLAIN,
				font_scale,						//fontScale
				W - K,								//color
				font_thickness);				//thickness
		}

		else {
			putText(*Map_font, to_string(i),
				plist[i] + shift2,
				FONT_HERSHEY_DUPLEX,			//FONT_HERSHEY_PLAIN,
				font_scale,						//fontScale
				W - K,								//color
				font_thickness);				//thickness
		}
	}



	bool drow(int i, int j) {
		if (i == num_obstacle | j == num_obstacle) {
			return false;
		}

		line(*Map_line, plist[i], plist[j], B, 4);

		return true;
	}



	void draw_all() {
		Node* np = Nhead;
		for (int i = 0; i < num_obstacle; i++) {
			int num = np->num;
			np = np->next;
			drow(num, np->num);
			//cout << num << "  " << np->num << endl;
		}
	}



	void erase_R(int i, int j) {

		line(*Map_erase, plist[i], plist[j], R, 4);
		line(*Map_line, plist[i], plist[j], Scalar::all(0), 4);

		//Queue
		erase_list.push_back(plist[i]);
		erase_list.push_back(plist[j]);

		return;
	}



	//relaxed
	void erase_k() {

		while(!erase_list.empty()) {

			//Queue
			Point p1 = erase_list.front();
			erase_list.pop_front();

			Point p2 = erase_list.front();
			erase_list.pop_front();

			line(*Map_erase, p1, p2, Scalar::all(0), 4); // K 로 하면 지운 선 표시 <-> Scalar::all(0)

		}
	}



	//2-opt
	void erase_P(int i, int j) {

		line(*Map_erase, plist[i], plist[j], P, 4);
		line(*Map_line, plist[i], plist[j], Scalar::all(0), 4);

		//Queue
		erase_list.push_back(plist[i]);
		erase_list.push_back(plist[j]);
	}



	bool drow_boruvka(int i, int j) {

		erase_k();

		if (i == num_obstacle | j == num_obstacle) {
			return false;
		}

		line(*Map_erase, plist[i], plist[j], B * 2 - R, 4);

		//Queue
		erase_list.push_back(plist[i]);
		erase_list.push_back(plist[j]);

		return drow(i, j);
	}



	//2-opt
	void switching_show(Node& n1, Node& n2) {

		erase_k();

		erase_P(n1.num, n1.next->num);
		erase_P(n2.num, n2.next->num);


		drow(n1.num, n2.num);
		drow(n2.next->num, n1.next->num);
	}




	//relaxed
	void switching_show(Node& n1, Node& n2, Node* n2n) {

		erase_k();

		erase_R(n1.previous->num, n1.num);
		
		erase_R(n2.previous->num, n2.num);
		erase_R(n2n->previous->num, n2n->num);


		drow(n1.previous->num, n2.num);
		drow(n2n->previous->num, n1.num);

		drow(n2.previous->num, n2n->num);
	}



	void init_show(Size _Map_size = Size(1000, 1000)) {

		namedWindow("TSP Map", WINDOW_AUTOSIZE);
		moveWindow("TSP Map", 800, 0);

		Map_frame = new Mat(_Map_size, CV_8UC3, K);
		Map_line = new Mat(_Map_size, CV_8UC3, Scalar::all(0));
		Map_erase = new Mat(_Map_size, CV_8UC3, Scalar::all(0));
		Map_font = new Mat(_Map_size, CV_8UC3, Scalar::all(0));

		//지정된 장애물이 없는 경우
		if (num_obstacle == -1) {
			cout << "\n ** show_TSP.h error : obstacle is NULL!!\n\n";
			return;
		}

		//장애물위치가 list에 저장되어있지 않은 경우
		if (plist == nullptr) {
			cout << "\n ** show_TSP.h error : plist is NULL!!\n\n";
		}


		for (int i = 0; i < num_obstacle; i++) {

			circle(*Map_frame, plist[i], radius, G, -1);
			circle(*Map_frame, plist[i], rds*1.5, G, 2);

			//arrowedLine(*Map_frame, plist[i], (Point2d)plist[i] + get_xy(TSP::map_vec[i]), W/3, 2, 8, 0, 0.3);

			drow(i);

		}//end for

		imshow("TSP Map", *Map_frame);

		return;

	}//end init_show



	// Boruvka()
	void start1() {
		cout << "\n\n	** start Boruvka() ** \n\n";

		erase_k();
		show("start1");
	}



	// relaxed_ones()
	void start2(int num = 0) {
		cout << "\n\n	** start relaxed_ones(" << num << ") ** \n\n";

		erase_k();
		show("start2");
	}



	// relaxed_3rd()
	void start3(int num = 0) {
		cout << "\n\n	** start relaxed_twice(" << num << ") ** \n\n";

		erase_k();
		show("start3");
	}




	// relaxed_3rd()
	void start4(int num = 0) {
		cout << "\n\n	** start relaxed_3rd(" << num << ") ** \n\n";

		erase_k();
		show("start4");
	}



	//  2 - opt()
	void start5(int num = 0) {
		cout << "\n\n	** start 2 - opt (" << num << ") ** \n\n";

		erase_k();
		show("start5");
	}




	void end() {
		cout << "\n ** *** ** ** *** ** ** *** ** \n";
		cout <<   " ** *** ** ** end ** ** *** ** \n";
		cout <<   " ** *** ** ** *** ** ** *** ** \n\n";
		
		get_cost();
		draw_all();

		erase_k();
		show("end");
	}



	//원형 노드용
	void print_Hamilton(Node* n) {

		//print_link_ones(n);
		//print_link_ones(n->next);

		printf("<%2d> - ", n->num);

		Node* np = n->next;
		Node* buf = np;

		while (np != n) {
			printf("%2d - ", np->num);

			np = np->next;

			if (np == nullptr) {
				printf("  ** Hamilton 오류 : loop 끊킴  Node : %d", np->num);
				return;
			}
			else if (np == np->next) {
				printf("  ** Hamilton 오류 : infinity loop  Node : %d", np->num);
				return;
			}
			else if (buf != np->previous) {
				printf("  ** Hamilton 오류 : infinity loop Node : %d", np->num);
				return;
			}
			buf = np;
		}
		printf("<%d>\n", n->num);
	}


}// End TSP 

#endif /// !EMOTION_H