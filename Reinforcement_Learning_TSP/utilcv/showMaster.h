#pragma once

#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "utilcv/Debug.h"
#include "utilcv/help_mat.h"



using namespace cv;
using namespace std;

namespace GA {
#define VISUALIZATION 1
}


namespace show {
	static int nwindow = 1000;
	//						init
	const bool flag[10] = { 1,		1, 1, 1, 1, 1, 1, 1, 1, 1 };
	

	void init(Mat img, int i = nwindow++) {
		if (VISUALIZATION && flag[0]) {
			imshow("init" + to_string(i), img);
		}
	}
}// End chan


#endif /// !EMOTION_H