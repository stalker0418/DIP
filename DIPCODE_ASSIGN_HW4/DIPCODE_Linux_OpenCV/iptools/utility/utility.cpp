#include "utility.h"
#include <cstdio>
#include <stdio.h>
#include <math.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#define MAXRGB 255
#define MINRGB 0

using namespace cv;

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}




/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cvtColor(src, tgt, COLOR_BGR2GRAY);
	imwrite("test_output.pgm",tgt);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	//blur(src,tgt,Size(WindowSize,WindowSize));
	GaussianBlur(src,tgt, Size(WindowSize,WindowSize), 0);
}

void utility::merge1(Mat &src, Mat &tgt,int x,int y, int Sx, int Sy){
	for (int i = x; i<x+Sx; i++) {
		for (int j=y; j<y+Sy; j++) {
			src.at<double>(i,j) = tgt.at<double>(i,j);
		}
	}	
}

/*-----------------------------------------------------------------------**/
void utility::cv_sobel(Mat &src, Mat &tgt, int KernelSize,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_gray;
		cvtColor(src, img_gray, COLOR_BGR2GRAY);
		GaussianBlur(img_gray,img_gray, Size(3,3), 0);

		Sobel(img_gray, tgt, CV_32F, 1, 1, KernelSize);
		
		if(KernelSize == 3)imwrite("test_sobel3.jpg",tgt);
		else imwrite("test_sobel5.jpg",tgt);

		//merge(src, tgt, x, y, Sx, Sy);
		//tgt = src.clone();
}

/*-----------------------------------------------------------------------**/
void utility::cv_coloredge(Mat &src, Mat &tgt, int KernelSize,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_value;
		cvtColor(src, src, COLOR_BGR2HSV);
		vector<Mat> hsv_channels;
		split(src,hsv_channels);
		img_value = hsv_channels[2];
		GaussianBlur(img_value(region),img_value(region), Size(3,3), 0);

		Sobel(img_value(region), tgt, CV_32F, 1, 1, KernelSize);
		
		if(KernelSize == 3)imwrite("coloredge_sobel3.pgm",tgt);
		else imwrite("coloredge_sobel5.pgm",tgt);

}

/*-----------------------------------------------------------------------**/
void utility::cv_binaryedge(Mat &src, Mat &tgt,int threshold, int angle,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_gray;
		Mat img_Dx;
		Mat img_Dy;
		Mat angle_Array;
		Mat tgt1;
		cvtColor(src, img_gray, COLOR_BGR2GRAY);
		GaussianBlur(img_gray(region),img_gray(region), Size(3,3), 0);

		Sobel(img_gray(region), img_Dx, CV_64F, 1, 0, 5);
		Sobel(img_gray(region), img_Dy, CV_64F, 0, 1, 5);
		Sobel(img_gray(region), tgt1, CV_64F, 1, 1, 5);
		cartToPolar(img_Dx,img_Dy,tgt1,angle_Array,true);

		
		//convertScaleAbs(img_Dx / 8, img_Dx);
		//convertScaleAbs(img_Dy / 8, img_Dy);
		//convertScaleAbs(tgt / 8, tgt);

		//imshow("output",img_Dx);
		//cout << img_Dx << '\n';
		//imwrite("dx.pgm",img_Dx);
		//imwrite("dy.pgm",img_Dy);
		//imwrite("after_nrom_tgt.pgm",tgt);
		

		//phase(img_Dx,img_Dy,angle_Array,true);
		//cout << angle_Array<< '\n';

		for (int i=0; i<tgt1.rows; i++) {
			for(int j=0;j<tgt1.cols;j++){
				if(abs(angle_Array.at<double>(i,j)-angle) >10)tgt1.at<double>(i,j) = 0;
			}
		}
		tgt1.convertTo(tgt,CV_32F);
		imwrite("binaryedge.pgm",tgt);
}


/*-----------------------------------------------------------------------**/
void utility::cv_canny(Mat &src, Mat &tgt,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_value;
		cvtColor(src, img_value, COLOR_BGR2GRAY);

		//GaussianBlur(img_value(region),img_value(region), Size(3,3), 0);
		GaussianBlur(img_value,img_value, Size(3,3), 0);
		//Canny(img_value(region),tgt,100,200,3,false);
		Canny(img_value,tgt,100,200,3,false);
		imwrite("canny.pgm",tgt);

}

/*-----------------------------------------------------------------------**/
void utility::cv_histoeq(Mat &src, Mat &tgt,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_value;
		cvtColor(src, img_value, COLOR_BGR2GRAY);

		//equalizeHist(img_value(region),tgt);
		equalizeHist(img_value,tgt);
		
		imwrite("histoeq.pgm",tgt);

}

/*-----------------------------------------------------------------------**/
void utility::cv_cannycv(Mat &src, Mat &tgt,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_value;
		Mat img_equalized;
		printf("entered function\n");
		cvtColor(src, img_value, COLOR_BGR2GRAY);

		equalizeHist(img_value(region),img_equalized);
		Canny(img_equalized,tgt,100,200,3,false);
		

		imwrite("equilaziedcanny.pgm",tgt);
}

/*-----------------------------------------------------------------------**/
void utility::cv_otsucv(Mat &src, Mat &tgt,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_value;
		
		cvtColor(src, img_value, COLOR_BGR2GRAY);

		//double thres = threshold(img_value(region),tgt,0,255,THRESH_OTSU);
		double thres = threshold(img_value,tgt,0,255,THRESH_OTSU);
		cout << thres << '\n';
		
		imwrite("otsuthreshold.pgm",tgt);

}

/*-----------------------------------------------------------------------**/
void utility::cv_otsuhisto(Mat &src, Mat &tgt,int x,int y,int Sx,int Sy)
{
		Rect region(x,y,Sy,Sx);
		Mat img_value;
		Mat background;
		
		cvtColor(src, img_value, COLOR_BGR2GRAY);

		//double thres = threshold(img_value(region),background,0,255,THRESH_OTSU);
		double thres = threshold(img_value,background,0,255,THRESH_OTSU);

		vector<int> bg;

		/*for (int i=0; i<img_value(region).rows; i++) {
			for(int j=0;j<img_value(region).cols;j++){
				int a = img_value.at<uchar>(i,j);
				if(a<=thres)bg.push_back(a);
			}
		}*/
		for (int i=0; i<img_value.rows; i++) {
			for(int j=0;j<img_value.cols;j++){
				int a = img_value.at<uchar>(i,j);
				if(a<=thres)bg.push_back(a);
			}
		}

		int size = bg.size();
		Mat background_mat(1,size,CV_8UC1,Scalar(0));

		for(int i=0;i<size;i++){
			background_mat.at<uchar>(0,i) = bg[i];
		}
		Mat equ_back;
		equalizeHist(background_mat,equ_back);

		int index=0;
		//tgt = img_value(region).clone();
		tgt = img_value.clone();
		/*for (int i=0; i<img_value(region).rows; i++) {
			for(int j=0;j<img_value(region).cols;j++){
				int a = img_value.at<uchar>(i,j);
				if(a<=thres){
					tgt.at<uchar>(i,j) = equ_back.at<uchar>(0,index);
					index++;
				}
			}
		}*/
		for (int i=0; i<img_value.rows; i++) {
			for(int j=0;j<img_value.cols;j++){
				int a = img_value.at<uchar>(i,j);
				if(a<=thres){
					tgt.at<uchar>(i,j) = equ_back.at<uchar>(0,index);
					index++;
				}
			}
		}



		
		//threshold(img_value(region),background,thres,255,THRESH_TOZERO_INV);
		
		imwrite("otsuhisto.pgm",tgt);

}


/*-----------------------------------------------------------------------**/
void utility::cv_QRcode(Mat &src, Mat &tgt)
{
		cout << "entered functino\n";
		QRCodeDetector qrcode;
		Mat points,img_result;
		//cvtColor(src, src, COLOR_BGR2GRAY);
		string data = qrcode.detectAndDecode(src,points,img_result);
		if(data.length() <=0){
			cout << "qr not detected\n";
			return;
		}
		cout << data << '\n';
		img_result.convertTo(img_result,CV_8UC3);
		imwrite("qrcode.jpg",img_result);
		img_result.copyTo(tgt);

}

/*--------------------------------------------------------------------------*/
void utility::cv_QRcodequl(Mat &src, Mat &tgt)
{
		cout << "entered function qrcode eq\n";
		QRCodeDetector qrcode;
		Mat points,img_result;
		Mat src1;
		cvtColor(src, src, COLOR_BGR2GRAY);
		equalizeHist(src,src1);
		string data = qrcode.detectAndDecode(src1,points,img_result);
		if(data.length() <=0){
			cout << "qr not detected\n";
			return;
		}
		cout << data << '\n';
		img_result.convertTo(img_result,CV_8UC3);
		imwrite("qrcodequl.jpg",img_result);
		img_result.copyTo(tgt);

}

/*--------------------------------------------------------------------------*/
void utility::cv_fourier(Mat &src, Mat &tgt,int x, int y, int Sx, int Sy)
{
		cout << "entered fourier function\n";
		Rect region(x,y,Sy,Sx);
		cvtColor(src, src, COLOR_BGR2GRAY);
		src.convertTo(src, CV_32F);	
		Mat I = src(region);
		Mat padded;
		int m = getOptimalDFTSize( I.rows );
		int n = getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

		Mat planes[] = {padded	, Mat::zeros(padded.size(), CV_32F)};
		Mat complexI;
		merge(planes, 2, complexI); 

		dft(complexI, complexI);     

		split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
		Mat magI = planes[0];
		
		magI += Scalar::all(1);                    // switch to logarithmic scale
		log(magI, magI);

		

		magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = magI.cols/2;
		int cy = magI.rows/2;
		Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
		Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);
		normalize(magI, magI, 0, 255, NORM_MINMAX); 
		//cout << magI << '\n';
		imwrite("fourier_output.jpg",magI);
		magI.copyTo(tgt);


}

/*--------------------------------------------------------------------------*/
void utility::cv_inverse_fourier(Mat &src, Mat &tgt,int x, int y, int Sx, int Sy)
{
		cout << "entered inverse fourier function\n";
		Rect region(x,y,Sy,Sx);
		cvtColor(src, src, COLOR_BGR2GRAY);
		src.convertTo(src, CV_32F);	
		Mat I = src(region);
		Mat padded;
		int m = getOptimalDFTSize( I.rows );
		int n = getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

		Mat planes[] = {padded	, Mat::zeros(padded.size(), CV_32F)};
		Mat complexI;
		merge(planes, 2, complexI); 

		dft(complexI, complexI);     
		
		






		split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

		Mat inverse;
		dft(complexI,inverse,DFT_INVERSE|DFT_REAL_OUTPUT);
		normalize(inverse,inverse,0,255,NORM_MINMAX);
		imwrite("inverse_fourier_output.jpg",inverse);
		inverse.copyTo(tgt);


}

/*--------------------------------------------------------------------------*/
/*Contains 4 filtering functions related to flag values
1. High Pass
2. Low Pass
3. Edge Sharpening
4. Edge Smoothing
*/
void utility::cv_filter(Mat &src, Mat &tgt,int x, int y, int Sx, int Sy,int t1, float t2, int flag)
{
		cout << "entered  filter function\n";
		//cout << x << y << Sx << Sy << '\n';
		Rect region(x,y,Sy,Sx);
		//cout << "maincheck1\n";
		cvtColor(src, src, COLOR_BGR2GRAY);
		//cout << "maincheck2\n";
		src.convertTo(src, CV_32F);	
		//Mat I = src(region);
		Mat I = src;
		//cout << "maincheck3\n";
		Mat padded;
		int m = getOptimalDFTSize( I.rows );
		int n = getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

		Mat planes[] = {padded	, Mat::zeros(padded.size(), CV_32F)};
		Mat complexI;
		Mat inverse;
		merge(planes, 2, complexI); 

		dft(complexI, complexI);  

		Mat H(complexI.size(), CV_32F, Scalar(1));
		for(int u=0;u < H.rows; u++){
			for(int v = 0;v<H.cols;v++){
				float D =(double)sqrt(pow((u-H.rows/2),2.0) + pow((v-H.cols/2), 2.0));
				if(flag == 1){//High Pass
					if(D <=t1)H.at<float>(u,v) = 0;
				}
				if(flag == 2){//Low Pass
					if(D> t1)H.at<float>(u,v) = 0;
				}
				if(flag == 3){//edge Sharpening
					if(D <=t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}
				if(flag ==4){// edge smoothing
					if(D> t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}



				//if(D <=t1 && flag )H.at<float>(u,v) = 0;
				//else if(!flag && D> t1)H.at<float>(u,v) = 0;
			}
		}
		//cout << "check1" << '\n';
		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = H.cols/2;
		int cy = H.rows/2;
		Mat q0(H, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		Mat q1(H, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(H, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(H, Rect(cx, cy, cx, cy)); // Bottom-Right
		Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);
		//cout << "check2\n";
		Mat planesH[] = { Mat_<float>(H.clone()), Mat_<float>(H.clone()) };
		Mat planes_dft[] = { complexI, Mat::zeros(complexI.size(), CV_32F) };
		split(complexI, planes_dft);

		Mat planes_out[] = { Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F) };
		planes_out[0] = planesH[0].mul(planes_dft[0]);
		planes_out[1] = planesH[1].mul(planes_dft[1]);

		merge(planes_out, 2, inverse);

		//cout << "check3\n";
		Mat inverse1;
		dft(inverse,inverse1,DFT_INVERSE|DFT_REAL_OUTPUT);
		normalize(inverse1,inverse1,0,255,NORM_MINMAX);
		inverse1.copyTo(tgt);
		//Mat inverse2;
		//cvtColor(inverse1, inverse2, COLOR_GRAY2BGR);
		if(flag==1)imwrite("highpass_fourier_output.jpg",inverse1);
		if(flag==2)imwrite("lowpass_fourier_output.jpg",inverse1);
		if(flag==3)imwrite("edgesharp_fourier_output.jpg",inverse1);
		if(flag==4)imwrite("edgesmooth_fourier_output.jpg",inverse1);
		


}



void utility::cv_hsvfilter_value(Mat &src, Mat &tgt,int x, int y, int Sx, int Sy,int t1, float t2, int flag)
{
		cout << "entered hsv filter function\n";
		Rect region(x,y,Sy,Sx);
		Mat Imhsv;
		Mat final ;
		tgt = src(region).clone();
		//cvtColor(src(region),Imhsv,COLOR_BGR2HSV);
		cvtColor(src,Imhsv,COLOR_BGR2HSV);
		
		//cvtColor(src,src,COLOR_BGR2GRAY);
		Mat hsv_channels[3];
		
		split(Imhsv,hsv_channels);

		hsv_channels[2].convertTo(hsv_channels[2], CV_32F);	
		Mat I = hsv_channels[2];
		
		Mat padded;
		int m = getOptimalDFTSize( I.rows );
		int n = getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
		cout << "lol\n";
		Mat planes[] = {padded	, Mat::zeros(padded.size(), CV_32F)};
		//cout << planes[1];
		Mat complexI;
		Mat inverse;
		cout << "checck before merge" << '\n';
		merge(planes, 2, complexI); 
		cout << "check after merge" << '\n';
		dft(complexI, complexI);  
		
		Mat H(complexI.size(), CV_32F, Scalar(1));
		for(int u=0;u < H.rows; u++){
			for(int v = 0;v<H.cols;v++){
				float D =(double)sqrt(pow((u-H.rows/2),2.0) + pow((v-H.cols/2), 2.0));
				if(flag == 1){//High Pass
					if(D <=t1)H.at<float>(u,v) = 0;
				}
				if(flag == 2){//Low Pass
					if(D> t1)H.at<float>(u,v) = 0;
				}
				if(flag == 3){//edge Sharpening
					if(D <=t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}
				if(flag ==4){// edge smoothing
					if(D> t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}

			}
		}

		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = H.cols/2;
		int cy = H.rows/2;
		Mat q0(H, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		Mat q1(H, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(H, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(H, Rect(cx, cy, cx, cy)); // Bottom-Right
		Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);

		Mat planesH[] = { Mat_<float>(H.clone()), Mat_<float>(H.clone()) };
		Mat planes_dft[] = { complexI, Mat::zeros(complexI.size(), CV_32F) };
		split(complexI, planes_dft);

		Mat planes_out[] = { Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F) };
		planes_out[0] = planesH[0].mul(planes_dft[0]);
		planes_out[1] = planesH[1].mul(planes_dft[1]);

		merge(planes_out, 2, inverse);

		Mat inverse1;
		dft(inverse,inverse1,DFT_INVERSE|DFT_REAL_OUTPUT);
		normalize(inverse1,inverse1,0,255,NORM_MINMAX);

		inverse1.convertTo(inverse1, CV_8U);
		inverse1.copyTo(hsv_channels[2]);
		hsv_channels[2].convertTo(hsv_channels[2],CV_8U);

		cout << inverse1.rows << ',' << inverse1.cols << '\n';
		cout << hsv_channels[2].rows << ',' << hsv_channels[2].cols << '\n';
		cout << hsv_channels[2].type() << '\n';
		merge(hsv_channels, 3, final);
		cvtColor(final,tgt,COLOR_HSV2BGR);

		if(flag==1)imwrite("hsv_highpass_fourier_output.jpg",tgt);
		if(flag==2)imwrite("hsv_lowpass_fourier_output.jpg",tgt);
		if(flag==3)imwrite("hsv_edgesharp_fourier_output.jpg",tgt);
		if(flag==4)imwrite("hsv_edgesmooth_fourier_output.jpg",tgt);
		cout << "lol4\n";

}




/*---------------------------------------------------------------------------------------*/
void utility::cv_hsvfilter_hue(Mat &src, Mat &tgt,int x, int y, int Sx, int Sy,int t1, float t2, int flag)
{
		cout << "entered hsv hue filter function\n";
		Rect region(x,y,Sy,Sx);
		Mat Imhsv;
		Mat final ;
		tgt = src(region).clone();
		cvtColor(src(region),Imhsv,COLOR_BGR2HSV);
		
		//cvtColor(src,src,COLOR_BGR2GRAY);
		Mat hsv_channels[3];
		
		split(Imhsv,hsv_channels);

		hsv_channels[0].convertTo(hsv_channels[0], CV_32F);	
		Mat I = hsv_channels[0];
		cout << hsv_channels[0] << '\n';
		cout << "type is" << hsv_channels[0].type() << '\n';
		Mat padded;
		int m = getOptimalDFTSize( I.rows );
		int n = getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
		cout << "lol\n";
		Mat planes[] = {padded	, Mat::zeros(padded.size(), CV_32F)};
		//cout << planes[1];
		Mat complexI;
		Mat inverse;
		cout << "checck before merge" << '\n';
		merge(planes, 2, complexI); 
		cout << "check after merge" << '\n';
		dft(complexI, complexI);  
		
		Mat H(complexI.size(), CV_32F, Scalar(1));
		for(int u=0;u < H.rows; u++){
			for(int v = 0;v<H.cols;v++){
				float D =(double)sqrt(pow((u-H.rows/2),2.0) + pow((v-H.cols/2), 2.0));
				if(flag == 1){//High Pass
					if(D <=t1)H.at<float>(u,v) = 0;
				}
				if(flag == 2){//Low Pass
					if(D> t1)H.at<float>(u,v) = 0;
				}
				if(flag == 3){//edge Sharpening
					if(D <=t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}
				if(flag ==4){// edge smoothing
					if(D> t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}

			}
		}

		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = H.cols/2;
		int cy = H.rows/2;
		Mat q0(H, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		Mat q1(H, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(H, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(H, Rect(cx, cy, cx, cy)); // Bottom-Right
		Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);

		Mat planesH[] = { Mat_<float>(H.clone()), Mat_<float>(H.clone()) };
		Mat planes_dft[] = { complexI, Mat::zeros(complexI.size(), CV_32F) };
		split(complexI, planes_dft);

		Mat planes_out[] = { Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F) };
		planes_out[0] = planesH[0].mul(planes_dft[0]);
		planes_out[1] = planesH[1].mul(planes_dft[1]);

		merge(planes_out, 2, inverse);

		Mat inverse1;
		dft(inverse,inverse1,DFT_INVERSE|DFT_REAL_OUTPUT);
		normalize(inverse1,inverse1,0,180,NORM_MINMAX);

		//inverse1.convertTo(inverse1, CV_8U);
		inverse1.copyTo(hsv_channels[0]);
		hsv_channels[0].convertTo(hsv_channels[0],CV_8U);

		cout << inverse1.rows << ',' << inverse1.cols << '\n';
		cout << hsv_channels[2].rows << ',' << hsv_channels[2].cols << '\n';
		cout << hsv_channels[0].type() << '\n';
		merge(hsv_channels, 3, final);
		cout << "lol3\n";
		cvtColor(final,tgt,COLOR_HSV2BGR);

		
		if(flag==1)imwrite("hsv_hue_highpass_fourier_output.jpg",tgt);
		if(flag==2)imwrite("hsv_hue_lowpass_fourier_output.jpg",tgt);
		if(flag==3)imwrite("hsv_hue_edgesharp_fourier_output.jpg",tgt);
		if(flag==4)imwrite("hsv_hue_edgesmooth_fourier_output.jpg",tgt);
		cout << "lol4\n";

}void utility::cv_hsvfilter_saturation(Mat &src, Mat &tgt,int x, int y, int Sx, int Sy,int t1, float t2, int flag)
{
		cout << "entered hsv filter saturation function\n";
		Rect region(x,y,Sy,Sx);
		Mat Imhsv;
		Mat final ;
		tgt = src(region).clone();
		cvtColor(src(region),Imhsv,COLOR_BGR2HSV);
		
		//cvtColor(src,src,COLOR_BGR2GRAY);
		Mat hsv_channels[3];
		
		split(Imhsv,hsv_channels);

		
		cout << hsv_channels[1] << '\n';
		cout << hsv_channels[1].type() << "is the type of saturation\n";

		hsv_channels[1].convertTo(hsv_channels[1], CV_32F);	
		Mat I = hsv_channels[1];
		
		Mat padded;
		int m = getOptimalDFTSize( I.rows );
		int n = getOptimalDFTSize( I.cols ); // on the border add zero values
		copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
		cout << "lol\n";
		Mat planes[] = {padded	, Mat::zeros(padded.size(), CV_32F)};
		//cout << planes[1];
		Mat complexI;
		Mat inverse;
		cout << "checck before merge" << '\n';
		merge(planes, 2, complexI); 
		cout << "check after merge" << '\n';
		dft(complexI, complexI);  
		
		Mat H(complexI.size(), CV_32F, Scalar(1));
		for(int u=0;u < H.rows; u++){
			for(int v = 0;v<H.cols;v++){
				float D =(double)sqrt(pow((u-H.rows/2),2.0) + pow((v-H.cols/2), 2.0));
				if(flag == 1){//High Pass
					if(D <=t1)H.at<float>(u,v) = 0;
				}
				if(flag == 2){//Low Pass
					if(D> t1)H.at<float>(u,v) = 0;
				}
				if(flag == 3){//edge Sharpening
					if(D <=t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}
				if(flag ==4){// edge smoothing
					if(D> t1)H.at<float>(u,v) = 0;
					H.at<float>(u,v) = H.at<float>(u,v) + t2 - 1 ;
				}

			}
		}

		// rearrange the quadrants of Fourier image  so that the origin is at the image center
		int cx = H.cols/2;
		int cy = H.rows/2;
		Mat q0(H, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		Mat q1(H, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(H, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(H, Rect(cx, cy, cx, cy)); // Bottom-Right
		Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);

		Mat planesH[] = { Mat_<float>(H.clone()), Mat_<float>(H.clone()) };
		Mat planes_dft[] = { complexI, Mat::zeros(complexI.size(), CV_32F) };
		split(complexI, planes_dft);

		Mat planes_out[] = { Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F) };
		planes_out[0] = planesH[0].mul(planes_dft[0]);
		planes_out[1] = planesH[1].mul(planes_dft[1]);

		merge(planes_out, 2, inverse);

		Mat inverse1;
		dft(inverse,inverse1,DFT_INVERSE|DFT_REAL_OUTPUT);
		normalize(inverse1,inverse1,0,255,NORM_MINMAX);

		//inverse1.convertTo(inverse1, CV_8U);
		inverse1.copyTo(hsv_channels[1]);
		hsv_channels[1].convertTo(hsv_channels[1],CV_8U);

		cout << inverse1.rows << ',' << inverse1.cols << '\n';
		cout << hsv_channels[2].rows << ',' << hsv_channels[2].cols << '\n';
		cout << hsv_channels[1].type() << '\n';
		merge(hsv_channels, 3, final);
		cvtColor(final,tgt,COLOR_HSV2BGR);

		if(flag==1)imwrite("hsv_saturation_highpass_fourier_output.jpg",tgt);
		if(flag==2)imwrite("hsv_saturation_lowpass_fourier_output.jpg",tgt);
		if(flag==3)imwrite("hsv_saturation_edgesharp_fourier_output.jpg",tgt);
		if(flag==4)imwrite("hsv_saturation_edgesmooth_fourier_output.jpg",tgt);
		cout << "lol4\n";

}





