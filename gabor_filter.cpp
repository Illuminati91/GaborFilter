#include<stdio.h>
#include<math.h>
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/core/core.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include<complex.h>
#include<sys/time.h>

#define PI 3.14159265
using namespace cv;

Mat carrier_signal(int imSize,int theta,float F,int phase) {
	float sind = sin(theta*PI/180);
	float cosd = cos(theta*PI/180);
	int i,j,x,y;
	int half = imSize/2;
	Mat im = Mat::zeros(imSize,imSize,CV_64F);

	for(i = 0;i < imSize;i++) {
		x = half - i;
		y = half;
		for(j = 0;j < imSize;j++) {
			im.at<double>(i,j) = creal(cexp(I*(2*PI*F*(x*cosd + (j-y)*sind)+ phase)));
		}
	}
	return im;
}

Mat gaussian_envelope(int imSize,int theta,float sigX,float sigY) {
	float sind = sin(theta*PI/180);
	float cosd = cos(theta*PI/180);
	sigX = sigX*sigX;
	sigY = sigY*sigY;
	int i,j,x,y;
	float x_t,y_t;
	int half = imSize/2;
	Mat im = Mat::zeros(imSize,imSize,CV_64F);

	for(i = 0;i < imSize;i++) {
		x = half - i;
		y = half;
		for(j = 0;j < imSize;j++) {
			x_t = pow(x*cosd + (j-y)*sind,2);
			y_t = pow(-1*x*sind + (j-y)*cosd,2);
			im.at<double>(i,j) = exp(-0.5*(x_t/sigX + y_t/sigY));
		}
	}
	return im;
}

Mat generate_Gabor_Kernel(int imSize,int theta,float F,int sigX,int sigY) {
	int i,j;
	Mat ca = carrier_signal(imSize,theta,F,0);
	Mat gau = gaussian_envelope(imSize,theta,sigX,sigY);
	Mat kernel = Mat::zeros(imSize,imSize,CV_64F);
	for(i = 0;i < imSize;i++) {
		for(j = 0;j < imSize;j++) {
			kernel.at<double>(i,j) = ca.at<double>(i,j)*gau.at<double>(i,j);
		}
	}
	return kernel;
}
