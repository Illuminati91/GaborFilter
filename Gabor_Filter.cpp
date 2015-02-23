#include<stdio.h>
#include<math.h>
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/core/core.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include<complex.h>
#include<sys/time.h>

#define PI 3.14159265
using namespace cv;

void displayImage(Mat im) {
	namedWindow("Disp",CV_WINDOW_AUTOSIZE);
	imshow("Disp",im);
	waitKey(0);
}

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

int main(int argc,char **argv) {
	const char *imgName;
	int i,j;
	double ST,ET;
	struct timeval st,et;
	long timeEl;
	Mat oim;

	if(argc > 1)
		imgName = argv[1];
	else {
		printf("Please give the image name along with path as input");
		exit(EXIT_FAILURE);
	}
	//Reading image
	Mat im = imread(imgName,CV_LOAD_IMAGE_GRAYSCALE);
	if(im == NULL) {
		printf("Image read fail!\n");
		exit(EXIT_FAILURE);
	}
	im.convertTo(im,CV_64F,1.0/255,0);
	
	//********* Parameters ******************
	int imSize = 20;
	float F = 0.08;
	int theta = 90;
	int phase = 0;
	int sigX = 7;
	int sigY = 20;
	//***************************************
	gettimeofday(&st,NULL);
	
	//Generating gabor kernel
	Mat ca = generate_Gabor_Kernel(imSize,theta,F,sigX,sigY);
	gettimeofday(&et,NULL);
	ST = st.tv_sec*100000 + (st.tv_usec);
	ET = et.tv_sec*100000 + (et.tv_usec);
	printf("Time taken to generate kernel: %ldms\n",(long)(ET-ST));

	//Convolving image and gabor kernel
	filter2D(im,oim,CV_64F,ca);

	//To display image side-by-side
	Size sz = im.size();
	Mat dispIm(sz.height,2*sz.width,CV_64F);
	Mat left(dispIm,Rect(0,0,sz.width,sz.height));
	im.copyTo(left);
	Mat right(dispIm,Rect(sz.width,0,sz.width,sz.height));
	oim.copyTo(right);
	displayImage(dispIm);
	return 0;
}
