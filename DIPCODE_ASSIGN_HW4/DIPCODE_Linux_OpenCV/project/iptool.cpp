#include "../iptools/core.h"
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <vector>


using namespace std;

using namespace cv;

#define MAXLEN 256

int flag = 0;

void merge(vector<Mat>roi_Array,vector<int>X,vector<int> Y,Mat &src, Mat& tgt, int flag){
	cout << "entered merge" << roi_Array.size() << "\n";
	if(!flag)cvtColor(src,src,COLOR_BGR2GRAY);
	tgt = src.clone();
	int i=0;
	
	while(i<roi_Array.size()){
		Mat temp;
		//cout << roi_Array[i] << '\n';
		cout << src.channels() << '\n';
		cout << roi_Array[i].channels() << '\n';
		cout << X[i] << ',' << Y[i] << ',' << roi_Array[i].cols << ',' << roi_Array[i].rows << '\n';
		roi_Array[i].copyTo(temp);
		temp.copyTo(tgt(Rect(X[i],Y[i],roi_Array[i].cols,roi_Array[i].rows)));
		cout << X[i] << ',' << Y[i] << ',' << roi_Array[i].cols << ',' << roi_Array[i].rows << '\n';
		i++;
	}
	
}




int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	char function_name[MAXLEN][MAXLEN];
	
	
	
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		if (strncmp(str,"#",1)==0) continue;
		int NumParameters = 0;
		
		pch = strtok(str, " ");
		strcpy(infile, pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");
		int j = 0,roi = 0;

		vector<Mat> roi_array;
		vector<int> X;
		vector<int> Y;
		Mat final_opencv_img;
		
		//cout << pch << ',' << strcmp(pch, "QRcode\n") << '\n';
 		if (strncasecmp(pch,"QRcode",6)==0 ){
			cv::Mat qrcode_img = cv::imread(infile);
			cv::Mat tgt1;
			
			if(qrcode_img.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}
			if (strncasecmp(pch,"QRcodequl",9)==0) utility::cv_QRcodequl(qrcode_img, tgt1);
			else utility::cv_QRcode(qrcode_img,tgt1);
			cv::imwrite(outfile, tgt1);
		}

		else if (strncasecmp(pch,"opencv",6)==0 ){
			cout << "entered opencv\n";
			cv::Mat src1 = cv::imread(infile);
			
			int merge_flag = 1;
			
			if(src1.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}
			pch = strtok(NULL, " ");
			int roi1 = atoi(pch);
			int j1 = 0;
			vector<int> X1,Y1,Sx1,Sy1;
			vector<Mat> roi_array1;
			int opencv_flag = 1;
			while(j1 < roi1){
				cv::Mat src1 = cv::imread(infile);
				cv::Mat tgt1;
				cout << "entered ;loop\n";
				int x = atoi(strtok(NULL, " "));
				int y = atoi(strtok(NULL, " "));
				int Sx = atoi(strtok(NULL, " "));
				int Sy = atoi(strtok(NULL, " "));
				X1.push_back(x);
				Y1.push_back(y);
				Sx1.push_back(Sx);
				Sy1.push_back(Sy);
				char* func_name = strtok(NULL, " ");
				//cout << strncasecmp(func_name,"mag",3) << '\n';
				if(strncasecmp(func_name,"dft",3) == 0){
					opencv_flag = 0;
					merge_flag = 0;
					utility::cv_fourier(src1,tgt1,x,y,Sx,Sy);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
					cv::imwrite(outfile,tgt1);
					
				}
				else if(strncasecmp(func_name,"invdft",6) == 0){
					opencv_flag = 0;
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					utility::cv_inverse_fourier(src1,tgt1,x,y,Sx,Sy);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
					
				}
				else if(strncasecmp(func_name,"lowpass",7) == 0){
					opencv_flag = 0;
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					utility::cv_filter(src1,tgt1,x,y,Sx,Sy,t1,0,2);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
					cout << "cameout\n";
				}
				else if(strncasecmp(func_name,"highpass",8) == 0){
					opencv_flag = 0;
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					utility::cv_filter(src1,tgt1,x,y,Sx,Sy,t1,0,1);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
				}
				else if(strncasecmp(func_name,"edgesharp",9) == 0){
					opencv_flag = 0;
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					float t2 = stof(strtok(NULL, " "));
					utility::cv_filter(src1,tgt1,x,y,Sx,Sy,t1,t2,3);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
				}
				else if(strncasecmp(func_name,"edgesmooth",10) == 0){
					opencv_flag = 0;
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					float t2 = stof(strtok(NULL, " "));
					utility::cv_filter(src1,tgt1,x,y,Sx,Sy,t1,t2,4);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
				}
				else if(strncasecmp(func_name,"hsvlowpass_value",16) == 0){
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					//float t2 = stof(strtok(NULL, " "));
					utility::cv_hsvfilter_value(src1,tgt1,x,y,Sx,Sy,t1,0,2);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
				}
				else if(strncasecmp(func_name,"hsvhighpass_value",17) == 0){
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					//float t2 = stof(strtok(NULL, " "));
					utility::cv_hsvfilter_value(src1,tgt1,x,y,Sx,Sy,t1,0,1);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
				}
				else if(strncasecmp(func_name,"hsvhighpass_hue",15) == 0){
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					//float t2 = stof(strtok(NULL, " "));
					utility::cv_hsvfilter_hue(src1,tgt1,x,y,Sx,Sy,t1,0,1);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
				}
				else if(strncasecmp(func_name,"hsvhighpass_saturation",22) == 0){
					//cout << "entered ag\n";
					//cout << x << y << Sx << Sy << '\n';
					int t1 = atoi(strtok(NULL, " "));
					//float t2 = stof(strtok(NULL, " "));
					utility::cv_hsvfilter_saturation(src1,tgt1,x,y,Sx,Sy,t1,0,1);
					roi_array1.push_back(tgt1);
					//cout << tgt1 << '\n';
				}

				j1++;
			}
			if (merge_flag) {
				merge(roi_array1,  X1,  Y1, src1, final_opencv_img,opencv_flag);
				cv::imwrite(outfile,final_opencv_img);
			}
			
			

			//utility::cv_fourier(src1,tgt1);
			//if(cv::imwrite(outfile,tgt1))cout << "works0\n";
			//else cout << "exception\n";
		}

		else roi = atoi(pch);
		cv::Mat I = cv::imread(infile);
		cv::Mat I3;
		while(j<roi){
			//cout << "entered" << '\n';
			int x = atoi(strtok(NULL, " "));
			int y = atoi(strtok(NULL, " "));
			int Sx = atoi(strtok(NULL, " "));
			int Sy = atoi(strtok(NULL, " "));
			X.push_back(x);
			Y.push_back(y);
			pch = strtok(NULL, " ");
			strcpy(function_name[j],pch);
			
			
			
			cv::Mat I2;
			
			//cout << pch << "is the function name \n";

			if( I.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}
			
			if (strcmp(pch,"gray")==0) {
				utility::cv_gray(I, I2);
			}
			else if (strcmp(pch,"gray1")==0) {
				utility::cv_gray(I, I2);
			}
			else if (strcmp(pch,"gra2y")==0) {
				utility::cv_gray(I, I2);
			}
			else if (strcmp(function_name[j],"blur_avg")==0) {
				pch = strtok(NULL, " ");
				utility::cv_avgblur(I, I2, atoi(pch));
				cv::imwrite(outfile,I2);
			}

			else if (strncasecmp(function_name[j],"sobel3",6)==0) {
				//pch = strtok(NULL, " ");
				utility::cv_sobel(I, I2, 3,x,y,Sx,Sy);
				//cout << "done\n";
				cv::imwrite(outfile,I2);
				roi_array.push_back(I2);
			}

			else if (strncasecmp(function_name[j],"sobel5",6)==0) {
				//pch = strtok(NULL, " ");
				utility::cv_sobel(I, I2, 5,x,y,Sx,Sy);
				cv::imwrite(outfile,I2);
				roi_array.push_back(I2);
			}
			
			

			else if (strncasecmp(function_name[j],"coloredge",9)==0) {
				//pch = strtok(NULL, " ");
				utility::cv_coloredge(I, I2, 5,x,y,Sx,Sy);
				roi_array.push_back(I2);
			}

			else if (strncasecmp(function_name[j],"binaryedge",10)==0) {
				int a = atoi(strtok(NULL, " "));
				int b = atoi(strtok(NULL, " "));
				//cout << "about to enter\n";
				utility::cv_binaryedge(I, I2,a,b,x,y,Sx,Sy);
				cout << "done\n";
				roi_array.push_back(I2);
			}
			
			else if (strncasecmp(function_name[j],"canny",5)==0) {
				//pch = strtok(NULL, " ");
				cout << "canny\n";
				if (strncasecmp(function_name[j],"cannycv",7)==0) {
					//pch = strtok(NULL, " ");
					cout << "cannycv check\n";
					utility::cv_cannycv(I,I2,x,y,Sx,Sy);
					
				}
				else utility::cv_canny(I,I2,x,y,Sx,Sy);
				cv::imwrite(outfile,I2);
				roi_array.push_back(I2);
			}

			else if (strncasecmp(function_name[j],"histoeq",7)==0) {
				//pch = strtok(NULL, " ");
				printf("check2\n");	
				utility::cv_histoeq(I,I2,x,y,Sx,Sy);
				cv::imwrite(outfile,I2);
				roi_array.push_back(I2);
			}	

			/*else if (strncasecmp(function_name[j],"cannycv",7)==0) {
				//pch = strtok(NULL, " ");
				cout << "cannycv check\n";
				utility::cv_cannycv(I,I2,x,y,Sx,Sy);
			}*/

			else if (strncasecmp(function_name[j],"otsucv",6)==0) {
				//pch = strtok(NULL, " ");
				utility::cv_otsucv(I,I2,x,y,Sx,Sy);
				cv::imwrite(outfile,I2);
				roi_array.push_back(I2);
			}

			else if (strncasecmp(function_name[j],"otsuhisto",9)==0) {
				//pch = strtok(NULL, " ");
				utility::cv_otsuhisto(I,I2,x,y,Sx,Sy);
				cv::imwrite(outfile,I2);
				roi_array.push_back(I2);
			}
			
			else if (strncasecmp(function_name[j],"add",3)==0) {
				src.read(infile);
				pch = strtok(NULL, " ");
				utility::addGrey(src,tgt,atoi(pch));
				flag = 1;
			}

			else if (strncmp(function_name[j],"binarize",8)==0) {
				src.read(infile);
				pch = strtok(NULL, " ");
				utility::binarize(src,tgt,atoi(pch));
				flag = 1;
			}

			else if (strncmp(function_name[j],"scale",5)==0) {
				src.read(infile);
				pch = strtok(NULL, " ");
				utility::scale(src,tgt,atof(pch));
				flag = 1;
			}

			else {
				printf("No function: %s\n", pch);
				continue;
			}
			
			if(flag)tgt.save(outfile);
			//else cv::imwrite(outfile, I2);
			
			j++;
		}
		//merge(roi_array,  X,  Y, I, I3);
		//if(!flag)cv::imwrite(outfile,I3);
			
	}
	fclose(fp);
	return 0;
}

