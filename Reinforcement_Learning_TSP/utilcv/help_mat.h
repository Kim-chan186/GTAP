#pragma once
#ifndef HELP_MAT_H
#define HELP_MAT_H


#include <opencv2/core.hpp>
#include "utilcv/showMaster.h"

cv::Scalar B(240, 143, 50);
cv::Scalar G(81, 247, 160);
cv::Scalar R(130, 20, 240);
cv::Scalar W(240, 240, 240);
cv::Scalar K(13, 13, 13);
cv::Scalar P(173, 103, 160);

namespace GA {
#define SGROUP	0.10	// 10%
#define LGROUP	0.80	// 80%-10% = 70%
						// (mutation) : 10%
#define LIN		10		// 1/10
	int LK = LIN;		
#define COSTGAIN 0.5

#define AUTOSET "AUTOSET"
#define MATLAB  "Node0128_4.txt"

#define MAPSIZE cv::Size(1000, 1000)
#define MAXCOST 999999

	int radius = 10;
	int line_thin = 5;
	int edge_thin = 3;
	cv::Scalar sc_circle(81, 247, 160);
	cv::Scalar sc_line = cv::Scalar::all(30);
	cv::Scalar sc_edge = cv::Scalar(240, 143, 50) - sc_line;

	//const int thresold_neighbor = sqrt(MAPSIZE.height * MAPSIZE.height + MAPSIZE.width * MAPSIZE.width);
}

//방법1 : 최대값을 255로 정규화 V
//방법2 : 나누기4 하는 방법
void check_8UC1(cv::Mat& data){
	if (data.type() == CV_8UC1) {
		return;
	}
	cv::normalize(data, data, 0, 255, cv::NORM_MINMAX, CV_8UC1, cv::noArray());
}


// img map의 빨간색 노드 좌표를 파일로 저장, 크기를 1000보다 작게 조절
void map2node(std::string map_name = "lin318small", std::string map_extension = ".png") {
	using namespace std;
	using namespace cv;
	
	Mat map = imread(map_name + map_extension), mask, stats, centroids, label;
	double size = map.cols * map.rows - 1;
	Size mSize = MAPSIZE;
	if (size > mSize.area()) {
		size = 1000 / sqrt(size);
	}
	else {
		size = sqrt(mSize.area() / size);
	}
	cv::resize(map, map, cv::Size(int(map.cols * size), int(map.rows * size)));
	std::cout << "image size : " << Size(map.cols, map.rows) << std::endl;
	show::init(map);

	inRange(map, Scalar(0, 0, 200), Scalar(205, 200, 255), mask);
	erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	int numOfLables = connectedComponentsWithStats(mask, label, stats, centroids, 8, CV_32S);
	
	show::init(mask);
	map -= Scalar::all(255);
	Vec3d color(255, 255, 255);
	cout << "num of node : " << numOfLables;
	ofstream fout(map_name + ".txt");
	for (int j = 1; j < numOfLables; j++) {
		int left = stats.at<int>(j, CC_STAT_LEFT);
		int top = stats.at<int>(j, CC_STAT_TOP);
		int width = stats.at<int>(j, CC_STAT_WIDTH);
		int height = stats.at<int>(j, CC_STAT_HEIGHT);
		map.at<Vec3b>(top + height / 2, left + width / 2) = color;
		fout << left + width / 2 << " " << top + height / 2 << endl;
	}
	fout.close();

	show::init(map);
	pause;
}


// auto thresholding 기능 추가 요망
// 이미지를 불러와서 이진맵(Otsu)을 반환하는 함수
// 이미지 크기가 너무 크면 적당히 줄임
cv::Mat getBinaryMap(std::string colorMap_name = "map_outline.jpg") {
	cv::Mat gframe;
	cv::cvtColor(cv::imread(colorMap_name), gframe, cv::COLOR_BGR2GRAY);
	
	double size = gframe.cols * gframe.rows - 1;
	Size mSize = MAPSIZE;
	if (size > mSize.area()) {
		size = 1000 / sqrt( size );
	}
	else {
		size = sqrt(mSize.area() / size);
	}
	cv::resize(gframe, gframe, cv::Size(int(gframe.cols * size), int(gframe.rows * size)));
	std::cout << "image size : " << Size(gframe.cols, gframe.rows) << std::endl;
	
	cv::threshold(gframe, gframe, 0, 255, cv::THRESH_OTSU);
	Mat morph = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::dilate(gframe, gframe, morph);
	cv::erode(gframe, gframe, morph);
	cv::erode(gframe, gframe, morph);
	cv::dilate(gframe, gframe, morph);
	show::init(gframe,1);
	return gframe;
}

#endif /// !EMOTION_H