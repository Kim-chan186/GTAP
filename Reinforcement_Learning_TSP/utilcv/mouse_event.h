#pragma once
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


using namespace cv;
using namespace std;


//Mat* cam_img = &Mat();

int count_moust = 0;



///포인터로 바꾸기
// uchar __ = __.color.ptr<uchar>(x)
// uchar __ = __.color.ptr<uchar>(y)
///데이터 직접접근으로 바꾸기
// uchar __ = __.data
void Mouse_Event(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		std::cout << "\tPoint(" << x << ", " << y << ")," << endl;
		
		//((Point*)userdata)[count_moust] = Point(x, y);

		//count_moust++;
	}

	if (event == EVENT_RBUTTONDOWN) {

		//uint r = cam_img->at<Vec3b>(y, x)[2];
		//uint g = cam_img->at<Vec3b>(y, x)[1];
		//uint b = cam_img->at<Vec3b>(y, x)[0];

		//std::cout << "     ┌ < Right Mouse Event >┐     " << endl;
		//std::cout << "     │     " << r << ", " << g << ", " << b << endl;
		//std::cout << "     └────────         " << endl;

	}
}



void set_moust(void* user) {
	
	namedWindow("TSP Map");
	setMouseCallback("TSP Map", Mouse_Event, user);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//https://kgh1994.tistory.com/4?category=694173





	
#endif /// !EMOTION_H