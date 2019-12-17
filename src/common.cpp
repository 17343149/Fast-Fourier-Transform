/**
 * @file common.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "common.h"

/**
 * @brief (-1)^x + y, make img in center
 * 
 * @param img 
 */
void makeImgInCenter(Mat &img){
    int width = img.cols;
    int height = img.rows;
    img.convertTo(img, CV_32FC1);
    for(int i = 0; i < height; ++i)
        for(int j = 0; j < width; ++j)
            if((i + j) & 1) img.at<float>(i, j) = -img.at<float>(i, j);
}

/**
 * @brief make the img width and height be the 2^n(for FFT)
 * 
 * @param img 
 */
void findProperSize(Mat &img){
    int width = img.cols;
    int height = img.rows;
    for(int i = 1; i <= 16; i *= 2){
        width |= width >> i;
        height |= height >> i;
    }
    width += 1, height += 1;
    resize(img, img, Size(width, height));
}

/**
 * @brief Use log() to calculate the length in frequency
 *        And record the max length
 * 
 * @param arg 
 * @param length 
 * @param max 
 */
void calculateLength(fftPair *arg, float **length, float &max){
    int width = arg->img.rows;
    int height = arg->img.cols;
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            length[i][j] = log(1.0f + sqrt(pow(arg->result_real[j][i], 2) + pow(arg->result_complex[j][i], 2)));
            if(max < length[i][j]) max = length[i][j];
        }
    }
}

/**
 * @brief Generate the image in frequency depend on the length and max
 * 
 * @param width 
 * @param height 
 * @param length 
 * @param max 
 * @return Mat 
 */
Mat generateFrequencyImg(int width, int height, float **length, const float max){
    Mat res(height, width, CV_8UC1, Scalar(0));
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            res.at<uchar>(i, j) = 0.5f + length[i][j] * 255 / max;
        }
    }
    return res;
}

/**
 * @brief Get the time now(ms)
 * 
 * @return int64_t 
 */
int64_t getTimeNow(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/**
 * @brief Calculate the value of W in x and y
 * 
 * @param width 
 * @param height 
 * @param width_W_real 
 * @param width_W_complex 
 * @param height_W_real 
 * @param height_W_complex 
 * @param sign
 */
void calculateW(int width, int height, float *width_W_real, float *width_W_complex, float *height_W_real, float *height_W_complex, float sign){
    const double double_pi = 2 * M_PI;
    for(int i = 0; i < width; ++i){
        width_W_real[i] = cos(double_pi * i / width);
        width_W_complex[i] = sign * sin(double_pi * i / width);
    }
    for(int i = 0; i < height; ++i){
        height_W_real[i] = cos(double_pi * i / height);
        height_W_complex[i] = sign * sin(double_pi * i / height);
    }
}

/**
 * @brief Invert the sign of every element in matrix
 * 
 * @param width 
 * @param height 
 * @param matrix 
 */
void invertSign(int width, int height, float **matrix){
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            matrix[i][j] = -matrix[i][j];
        }
    }
}