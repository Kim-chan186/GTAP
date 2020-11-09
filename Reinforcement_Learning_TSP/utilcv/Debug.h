#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <algorithm>
#include <memory>
#include <functional>

#define TTT  printf("\x1b[32m ** TEST0 ** \x1b[0m \n");
#define TTT1 printf("\x1b[32m ** TEST1 ** \x1b[0m \n");
#define TTT2 printf("\x1b[32m ** TEST2 ** \x1b[0m \n");
#define TTT3 printf("\x1b[32m ** TEST3 ** \x1b[0m \n");
#define TTT4 printf("\x1b[32m ** TEST4 ** \x1b[0m \n");

#define STT  printf("\x1b[36m \t** sub TEST0 ** \x1b[0 \n");
#define STT1 printf("\x1b[36m \t** sub TEST1 ** \x1b[0m \n");
#define STT2 printf("\x1b[36m \t** sub TEST2 ** \x1b[0m \n");
#define STT3 printf("\x1b[36m \t** sub TEST3 ** \x1b[0m \n");
#define STT4 printf("\x1b[36m \t** sub TEST4 ** \x1b[0m \n");

#define nn   printf("\n")
#define pause   waitKey(0)

//setw(2) <<
//\x1b[31m

namespace debug {


	int roop = 1;
	int calling = 1;

	void sys_out() {
		quick_exit(EXIT_SUCCESS);
	}

	void VERSION() {
		if (CV_VERSION == "4.1.2") {
			std::cout << "** OpcnCV Version is 4.1.2 \n\n";
		}
		else {
			std::cout << "** OpcnCV Version is NOT 4.1.2\n"
				<< "** OpcnCV Version is " << CV_VERSION << std::endl;
		}
	}

	// num 은 pointer a의 길이
	template <typename T>
	void print_pointer(T* a, int num, std::string code = "") {
		std::cout << " ** debug :: printf_pointer !! " << code << "\n";
		for (int i = 0; i < num; i++) {
			std::cout << a[i] << " ";
		}std::cout << "\n ** debug :: printf_pointer ** end \n\n";
	}

	template <typename T>
	void print_vec(std::vector<T> vec, std::string code = "") {
		std::cout << " ** debug :: printf_vec!! " << code << "\n";
		for (auto& item : vec) {
			std::cout << item << " ";
		}std::cout << "\n ** debug :: printf_vec ** end \n\n";
	}

	void empty(cv::Mat& data) {
		std::cout << calling++ << ". debug empty : " << data.empty() << std::endl;
	}



	void empty(cv::Mat& data, cv::Mat& data2) {
		std::cout << calling++ << ". debug empty : " << data.empty() << data2.empty() << std::endl;
	}



	void empty(cv::Mat& data, cv::Mat& data2, cv::Mat& data3) {
		std::cout << calling++ << ". debug empty : " << data.empty() << data2.empty() << data3.empty() << std::endl;
	}



	void empty(cv::Mat& data, cv::Mat& data2, std::vector<cv::DMatch> data3) {
		std::cout << calling++ << ". debug empty : " << data.empty() << data2.empty() << data3.empty() << std::endl;
	}



	//옆으로 붙이기
	void show(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		//cv::moveWindow("debug", 0, 0);
		//cv::hconcat(data, data2, show);
		cv::imshow("debug:show1", data);
		cv::imshow("debug:show2", data2);
	}



	void show_wait(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		//cv::moveWindow("debug", 0, 0);
		//cv::hconcat(data, data2, show);
		cv::imshow("debug:show_wait1", data);
		cv::imshow("debug:show_wait2", data2);
		cv::waitKey(0);
	}



	//아래로 붙이기
	//반복문으로 할껄...
	void show_compare(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		//cv::moveWindow("debug", 0, 0);
		//cv::hconcat(data, data2, show);
		cv::vconcat(data, data2, show);
		cv::imshow("debug:show_compare", show);
	}



	void show_compare(cv::Mat& data, cv::Mat& data2, cv::Mat& data3) {
		cv::Mat show;
		cv::vconcat(data, data2, show);
		cv::vconcat(show, data3, show);
		cv::imshow("debug:show_compare", show);
	}



	void show_compare(cv::Mat& data, cv::Mat& data2, cv::Mat& data3, cv::Mat& data4) {
		cv::Mat show;
		cv::vconcat(data, data2, show);
		cv::vconcat(show, data3, show);
		cv::vconcat(show, data4, show);
		cv::imshow("debug:show_compare", show);
	}



	void show_compare(cv::Mat& data, cv::Mat& data2, cv::Mat& data3, cv::Mat& data4, cv::Mat& data5) {
		cv::Mat show;
		cv::vconcat(data, data2, show);
		cv::vconcat(show, data3, show);
		cv::vconcat(show, data4, show);
		cv::vconcat(show, data5, show);
		cv::imshow("debug:show_compare", show);
	}



	//data : 이미지 배열, show : buffer_img, n : 배열 크기
	void show_compare(cv::Mat* data, cv::Mat& show, unsigned int n){
		if (show.empty()) {
			show = data[n];
		}
		else {
			cv::vconcat(data[n], show, show);
		}
		if (n) {
			show_compare(data, show, n - 1);
		}
		else {
			cv::imshow("debug:show_compare", show);
		}
	}



	void show_blending(cv::Mat& data, cv::Mat& data2) {
		cv::Mat show;
		cv::addWeighted(data, 0.5, data2, 0.5, 0, show);
		cv::imshow("debug:show_blending", show);
	}



	std::string Mat_type[8] = {
		"CV_8U"//   0
		,"CV_8S"//   1
		,"CV_16U"//  2
		,"CV_16S"//  3
		,"CV_32S"//  4
		,"CV_32F"//  5
		,"CV_64F"//  6
		,"CV_16F"//  7
	};



	void check_Mat(cv::Mat& data) {
		std::cout << "\n >> debug check_Mat\n";
		if (data.empty()) {
			std::cout << "빈 이미지 입니다.\n";
			std::cout << " debug <<\n";
			return;
		}
		
		std::cout << "cols     : " << data.cols << std::endl
			 << "rows     : "<< data.rows << std::endl;
		std::cout << "channels : " << data.channels() << std::endl
			 << "type     : " << Mat_type[data.type()] << std::endl
			 << "depth    : " << data.depth() << std::endl;

		std::cout << " debug check_Mat <<\n";
	}



	//putText(frame_show, str,
	//	Point(frame_show.cols / 2, frame_show.rows - 10),
	//	FONT_HERSHEY_PLAIN,
	//	1,								//fontScale
	//	cv::Scalar(255, 255, 0),		//color
	//	1.5);							//thickness
	//FONT_HERSHEY_SIMPLEX = 0,			//!< normal size sans-serif font
	//FONT_HERSHEY_PLAIN = 1,			//!< small size sans-serif font
	//FONT_HERSHEY_DUPLEX = 2,			//!< normal size sans-serif font (more complex than FONT_HERSHEY_SIMPLEX)
	//FONT_HERSHEY_COMPLEX = 3,			//!< normal size serif font
	//FONT_HERSHEY_TRIPLEX = 4,			//!< normal size serif font (more complex than FONT_HERSHEY_COMPLEX)
	//FONT_HERSHEY_COMPLEX_SMALL = 5,	//!< smaller version of FONT_HERSHEY_COMPLEX
	//FONT_HERSHEY_SCRIPT_SIMPLEX = 6,	//!< hand-writing style font
	//FONT_HERSHEY_SCRIPT_COMPLEX = 7,	//!< more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
	//FONT_ITALIC = 16					//!< flag for italic font
}

#endif /// !EMOTION_H