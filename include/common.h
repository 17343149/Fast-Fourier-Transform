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

#ifndef _COMMON_H
#define _COMMON_H

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
 * @brief Used in fft and ifft.
 *        Scale of result_real and complex is width * height.
 * 
 */
struct fftPair{
    Mat img;
    float** result_real;
    float** result_complex;

    fftPair(Mat arg, bool is_print = true){
        img = arg.clone();
        findProperSize(img);
        makeImgInCenter(img);
        int width = img.cols;
        int height = img.rows;
        result_real = new float*[width];
        result_complex = new float*[width];
        for(int i = 0; i < width; ++i){
            result_real[i] = new float[height];
            result_complex[i] = new float[height];
            memset(result_real[i], 0, sizeof(float) * height);
            memset(result_complex[i], 0, sizeof(float) * height);
        }
        if(is_print){
            printf("img -> width: %d, height: %d\n", img.cols, img.rows);
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
void calculateW(int, int, float*, float*, float*, float*, float sign = -1.0f);

/**
 * @brief 
 * 
 */
void invertSign(int, int, float **);

#endif


