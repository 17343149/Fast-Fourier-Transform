/**
 * @file common.h
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <chrono>

using namespace cv;
using namespace std;
using namespace chrono;

struct fftPair{
    Mat img;
    float **result_real;
    float **result_complex;

    fftPair(Mat arg){
        img = arg.clone();
        int width = img.rows;
        int height = img.cols;
        result_real = new float*[width];
        result_complex = new float*[width];
        for(int i = 0; i < width; ++i){
            result_real[i] = new float[height];
            result_complex[i] = new float[height];
            memset(result_real[i], 0, sizeof(float) * height);
            memset(result_complex[i], 0, sizeof(float) * height);
        }
    }

    ~fftPair(){
        int width = img.rows;
        for(int i = 0; i < width; ++i){
            delete[]result_real[i];
            delete[]result_complex[i];
        }
        delete[]result_real;
        delete[]result_complex;
    }
};

/**
 * @brief 
 * 
 */
void makeImgInCenter(Mat&);

/**
 * @brief 
 * 
 */
void findProperSize(Mat&);

/**
 * @brief 
 * 
 */
void calculateLength(fftPair*, float**, float&);

/**
 * @brief 
 * 
 * @return Mat 
 */
Mat generateFrequencyImg(int, int, float**, const float);

/**
 * @brief Get the Time Now
 * 
 * @return int64_t 
 */
int64_t getTimeNow();

/**
 * @brief 
 * 
 */
void calculateW(int, int, float*, float*, float*, float*);


