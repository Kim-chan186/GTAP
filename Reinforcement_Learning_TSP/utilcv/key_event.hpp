#pragma once
#ifndef KEY_EVENT_H
#define KEY_EVENT_H


#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace chan {
	using namespace cv;


	//방향키 좌우 (<-, -> 화살표)
	//bool (*arrow_func)(bool) = cam::cam_brightness;


	int waitKeySuper(int num) {
		
		int key = waitKeyEx(num);
		if (key == -1) {
			return -1;
		}

		// space bar
		if (key == 32) {
			return 1;
		}
		//Enter
		else if (key == 13) {
			return 1;
		}
		//back space bar
		else if (key == 8) {
			return 1;
		}
		//ESC
		else if (key == 27) {
			//console  뒤로 가는 거로 변경!
			std::cout << "\n  ** ESC로 프로그램을 종료합니다. \n\n";
			quick_exit(EXIT_SUCCESS);
		}

		std::cout << "input Key = " << key << std::endl;

		//방향키 위     2490368
		if (key == 2490368) {
		}
		//방향키 아래   2621440
		else if (key == 2621440) {
		}
		//방향키 오른쪽  2555904
		else if (key == 2555904) {
			num++;
		}
		//방향키 왼쪽   2424832
		else if (key == 2424832) {
			num--;
		}


		//F1
		else if (key == 7340032){
		}
		//F2
		else if (key == 7405568) {
		}	
		//echo
		else if (key > 255) {
		}
		else {
		}



		return key;
	}//end waitKeySuper

	int waitKeySuper() {
		int i = 0;
		waitKeySuper(i);
		return 1;
	}


}//End_chan

#endif /// !EMOTION_H