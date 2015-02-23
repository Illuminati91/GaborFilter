#ifndef GF
#define GF
#include"opencv2/core/core.hpp"

#define PI 3.14159265
using namespace cv;

Mat generate_Gabor_Kernel(int imSize,int theta,float F,int sigX,int sigY);

#endif
