

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

#include "data_structure_TSP.h"
#include "frame.h"
#include "genetic.h"
#include "MST.hpp"
#include "show_TSP.h"

#include "utilcv/Debug.h"
#include "utilcv/help_mat.h"
#include "utilcv/key_event.hpp"
#include "utilcv/mouse_event.h"
#include "utilcv/service.h"
#include "utilcv/showMaster.h"

// #include <mat.h>		// matlab data file reading
// #include <matrix.h>
// https://m.blog.naver.com/PostView.nhn?blogId=aureagenus&logNo=120097579189&proxyReferer=https:%2F%2Fwww.google.com%2F

using namespace cv;
using namespace std;

int main(int, char**){
	////show 설정 출력하는 구문 만들기
	//map2node("node_map", ".jpg");
	//pause;

	int ms = 1;
	GA::population ob(AUTOSET);		//AUTOSET : lin 318
	//GA::population ob("1");
	for (;chan::waitKeySuper(ms) != 28 && service::Check_Time(1);) {
		ob.nextGenerate();
		if(!ms)	service::Check_Time(1);
	}
	//cv::VideoCapture cap(1);
	//cap.set(CAP_PROP_EXPOSURE, -6);
	//Mat img;
	//while (waitKey(1)) {
	//	cap >> img;
	//	imshow("test", img);
	//}
}