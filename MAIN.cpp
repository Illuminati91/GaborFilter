#include<stdio.h>
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include<sys/time.h>
#include"gabor_filter.h"

void displayImage(Mat im) {
	namedWindow("Disp",CV_WINDOW_AUTOSIZE);
	imshow("Disp",im);
	waitKey(0);
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
	if(im.data == NULL) {
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
