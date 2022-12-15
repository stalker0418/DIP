#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void cv_gray(cv::Mat &src, cv::Mat &tgt);
		static void cv_avgblur(cv::Mat &src, cv::Mat &tgt, int WindowSize);
		static void merge1(cv::Mat &src, cv::Mat &tgt,int x,int y,int Sx,int Sy);
		static void cv_sobel(cv::Mat &src, cv::Mat &tgt, int KernelSize,int x,int y,int Sx,int Sy);
		static void cv_coloredge(cv::Mat &src, cv::Mat &tgt, int KernelSize,int x,int y,int Sx,int Sy);
		static void cv_binaryedge(cv::Mat &src, cv::Mat &tgt, int threshold, int angle,int x,int y,int Sx,int Sy);
		static void cv_canny(cv::Mat &src, cv::Mat &tgt,int x,int y,int Sx,int Sy);
		static void cv_histoeq(cv::Mat &src, cv::Mat &tgt,int x,int y,int Sx,int Sy);
		static void cv_cannycv(cv::Mat &src, cv::Mat &tgt,int x,int y,int Sx,int Sy);
		static void cv_otsucv(cv::Mat &src, cv::Mat &tgt,int x,int y,int Sx,int Sy);
		static void cv_otsuhisto(cv::Mat &src, cv::Mat &tgt,int x,int y,int Sx,int Sy);
		static void cv_QRcode(cv::Mat &src, cv::Mat &tgt);
		static void cv_QRcodequl(cv::Mat &src, cv::Mat &tgt);
		static void cv_fourier(cv::Mat &src, cv::Mat &tgt,int x, int y, int Sx, int Sy);
		static void cv_inverse_fourier(cv::Mat &src, cv::Mat &tgt,int x, int y, int Sx, int Sy);
		static void cv_filter(cv::Mat &src, cv::Mat &tgt,int x, int y, int Sx, int Sy, int t1,float t2, int flag);
		static void cv_hsvfilter_value(cv::Mat &src, cv::Mat &tgt,int x, int y, int Sx, int Sy, int t1,float t2, int flag);
		static void cv_hsvfilter_hue(cv::Mat &src, cv::Mat &tgt,int x, int y, int Sx, int Sy, int t1,float t2, int flag);
		static void cv_hsvfilter_saturation(cv::Mat &src, cv::Mat &tgt,int x, int y, int Sx, int Sy, int t1,float t2, int flag);
};

#endif

