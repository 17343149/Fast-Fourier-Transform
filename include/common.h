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
#include <stdlib.h>
#include <time.h>

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
void findProperSize(Mat&, int height = -1, int width = -1);

/**
 * @brief Used in fft and ifft.
 *        Scale of result_real and complex is width * height.
 * 
 */
struct fftPair{
    Mat img;
    float** result_real[3];
    float** result_complex[3];

    fftPair(Mat arg, int img_height = -1, int img_width = -1, bool is_print = true){
        img = arg.clone();
        findProperSize(img, img_height, img_width);
        int width = img.cols;
        int height = img.rows;
        img.convertTo(img, CV_32FC3);
        for(int i = 0; i < 3; ++i){
            result_real[i] = new float*[width];
            result_complex[i] = new float*[width];
            for(int j = 0; j < width; ++j){
                result_real[i][j] = new float[height];
                result_complex[i][j] = new float[height];
                memset(result_real[i][j], 0, sizeof(float) * height);
                memset(result_complex[i][j], 0, sizeof(float) * height);
            }
        }
        if(is_print){
            printf("img -> width: %d, height: %d\n", img.cols, img.rows);
        }
    }

    ~fftPair(){
        int width = img.rows;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < width; ++j){
                delete[]result_real[i][j];
                delete[]result_complex[i][j];
            }
            delete[]result_real[i];
            delete[]result_complex[i];
        }
    }

    void printMatrix(){
        int width = img.cols;
        int height = img.rows;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                cout << "(" << i << ", " << j << ") -> (" << result_real[j][i] << ", " << result_complex[j][i] << ")" << endl;
            }
        }
    }

};

/**
 * @brief 
 * 
 * @return Mat 
 */
Mat generateFrequencyImg(fftPair*);

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

/**
 * @brief 
 * 
 */
void getRandSequence(vector<float> &vec, int low, int high);

#endif


