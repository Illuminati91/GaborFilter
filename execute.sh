#!/bin/bash


g++ gabor_filter.cpp MAIN.cpp -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_highgui -lopencv_core -lopencv_imgproc -lm -pthread
