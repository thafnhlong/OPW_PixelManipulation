#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>

using namespace std;
using namespace cv;


class Util {
public:
	template<class T> static T max(T a, T b) {
		if (a > b) {
			return a;
		}
		return b;
	}
	template<class T> static T min(T a, T b) {
		if (a < b) {
			return a;
		}
		return b;
	}
};

#define HEIGHT_SIZE 400 //pixel
#define PADDING_SIZE 10 //pixel
#define WIDTH_COL_SIZE_3C 1 //pixel
#define WIDTH_COL_SIZE_1C 2 //pixel