/**
 * @file main.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-08
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
 */
void calculateW(int width, int height, float *width_W_real, float *width_W_complex, float *height_W_real, float *height_W_complex){
    const double double_pi = 2 * M_PI;
    for(int i = 0; i < width; ++i){
        width_W_real[i] = cos(double_pi * i / width);
        width_W_complex[i] = -sin(double_pi * i / width);
    }
    for(int i = 0; i < height; ++i){
        height_W_real[i] = cos(double_pi * i / height);
        height_W_complex[i] = -sin(double_pi * i / height);
    }
}

/**
 * @brief This is not the fft interface, and the true interface is below.
 *        Transform row firstly, then transform column
 * 
 * @param img -> origin image
 * @param row -> IF row > 0 THEN transform row ELSE transform column
 * @param col -> choose the column to transform
 * @param idx -> the index of begin of array, the value is 0 at the beginning 
 * @param K -> the K in fourier transform, related to img row and column size 
 * @param step_scale -> the scale of one step, for example, 1, 3, 5, 7, step_scale = 2
 * @param arr_real -> this is the array that need to be saved to backtrace
 * @param arr_complex -> same to arr_real
 * @param width_W_real -> the value of W in the direction of row
 * @param width_W_complex 
 * @param height_W_real -> the value of W in the direction of column
 * @param height_W_complex 
 * @param once_real -> the data saved after transform row
 * @param once_complex
 * @param length -> the length in frequency
 * @param max -> max length in frequency
 */
void fft2d(const Mat &img, int row, int col, int idx, int K, int step_scale, 
            float *arr_real, float *arr_complex, 
            float *width_W_real, float *width_W_complex, 
            float *height_W_real, float *height_W_complex, 
            float **once_real, float **once_complex,
            float **length, float &max) {

    const double double_pi = 2 * M_PI;
    int width = img.cols, height = img.rows;
    if(K > 1){ // calculate even and odd
        K >>= 1;
        int next_step_scale = step_scale << 1;
        float *even_real = new float[K];
        float *even_complex = new float[K];
        float *odd_real = new float[K];
        float *odd_complex = new float[K];
        memset(even_real, 0, sizeof(float) * K);
        memset(even_complex, 0, sizeof(float) * K);
        memset(odd_real, 0, sizeof(float) * K);
        memset(odd_complex, 0, sizeof(float) * K);

        // even
        fft2d(img, row, col, idx, K, next_step_scale, even_real, even_complex, width_W_real, width_W_complex, height_W_real, height_W_complex, once_real, once_complex, length, max);
        
        // odd
        fft2d(img, row, col, idx + step_scale, K, next_step_scale, odd_real, odd_complex, width_W_real, width_W_complex, height_W_real, height_W_complex, once_real, once_complex, length, max);

        // f(x) = even(x) + odd(x) * W
        // f(x + K) = even(x) - odd(x) * W
        for(int i = 0; i < K; ++i){
            float temp_real, temp_complex;
            if(row >= 0){
                temp_real = odd_real[i] * width_W_real[i * step_scale] - odd_complex[i] * width_W_complex[i * step_scale];
                temp_complex = odd_real[i] * width_W_complex[i * step_scale] + odd_complex[i] * width_W_real[i * step_scale];
            }else{
                temp_real = odd_real[i] * height_W_real[i * step_scale] - odd_complex[i] * height_W_complex[i * step_scale];
                temp_complex = odd_real[i] * height_W_complex[i * step_scale] + odd_complex[i] * height_W_real[i * step_scale];
            }
            arr_real[i] = even_real[i] + temp_real;
            arr_complex[i] = even_complex[i] + temp_complex;
            arr_real[i + K] = even_real[i] - temp_real;
            arr_complex[i + K] = even_complex[i] - temp_complex;
        }
        delete[]even_real;
        delete[]even_complex;
        delete[]odd_real;
        delete[]odd_complex;
    } else{
        if(row >= 0){ // transform row
            arr_real[0] = img.at<float>(row, idx);
            arr_complex[0] = 0;
        }else{ // use the data to transform column
            arr_real[0] = once_real[idx][col];
            arr_complex[0] = once_complex[idx][col];
        }
    }
}

/**
 * @brief The interface of FFT.
 *        fftPair() includes Mat, float **real, float **complex
 *        Constructor -> fftPair(Mat)
 * 
 * @param arg -> fftPair
 * @return Mat -> the image in frequency
 */
Mat fft2d(fftPair *arg){
    int width = arg->img.cols, height = arg->img.rows;

    // calculate W
    float *width_W_real = new float[width]; // the W of row
    float *width_W_complex = new float[width];
    float *height_W_real = new float[height]; // the W of column
    float *height_W_complex = new float[height];
    calculateW(width, height, width_W_real, width_W_complex, height_W_real, height_W_complex);

    // 2d arrays variable in frequency domain
    float **once_real = new float*[height];
    float **once_complex = new float*[height];
    float **length = new float*[height];
    for(int i = 0; i < height; ++i){
        once_real[i] = new float[width];
        once_complex[i] = new float[width];
        length[i] = new float[width];
        memset(once_real[i], 0, sizeof(float) * width);
        memset(once_complex[i], 0, sizeof(float) * width);
        memset(length[i], 0, sizeof(float) * width);
    }

    float max = 0;

    // calculate passed time
    int64_t BEGIN_TIME = getTimeNow();

    // @brief Transform in row!
    for(int i = 0; i < height; ++i){
        fft2d(arg->img, i, 0, 0, width, 1, once_real[i], once_complex[i], width_W_real, width_W_complex, height_W_real, height_W_complex, once_real, once_complex, length, max);
    }

    // @brief Transform in column!
    for(int i = 0; i < width; ++i){
        fft2d(arg->img, -1, i, 0, height, 1, arg->result_real[i], arg->result_complex[i], width_W_real, width_W_complex, height_W_real, height_W_complex, once_real, once_complex, length, max);
    }

    // calculate passed time
    int64_t END_TIME = getTimeNow();
    printf("%f s\n", (END_TIME - BEGIN_TIME) / 1000.0);

    calculateLength(arg, length, max);
    Mat res = generateFrequencyImg(width, height, length, max);

    for(int i = 0; i < height; ++i){
        delete[]once_real[i];
        delete[]once_complex[i];
        delete[]length[i];
    }
    delete[]once_real;
    delete[]once_complex;
    delete[]length;

    return res;
}

/**
 * @brief IFFT
 * 
 * @param img 
 * @param result_real 
 * @param result_complex 
 * @return Mat 
 */
Mat ifft2d(Mat &img, float **result_real, float **result_complex){
    const double double_pi = 2 * M_PI;
    int width = img.cols, height = img.rows;
    int w_K = width / 2, h_K = height / 2;
    float **temp_real = new float*[height]; // save middle value
    float **temp_complex = new float*[height];
    Mat ifft_img(height, width, CV_8UC1, Scalar(0));
    for(int i = 0; i < height; ++i){
        temp_real[i] = new float[width];
        temp_complex[i] = new float[width];
        memset(temp_real[i], 0, sizeof(float) * width);
        memset(temp_complex[i], 0, sizeof(float) * width);
    }

    int64_t BEGIN_TIME = getTimeNow();
    for(int i = 0; i < height; ++i){ // y
        for(int j = 0; j < width; ++j){ // u
            float theta = 0;
            for(int y = 0; y < height; ++y){ // v
                theta = double_pi * i * y / height;
                temp_real[i][j] += result_real[y][j] * cos(theta) - result_complex[y][j] * sin(theta);
                temp_complex[i][j] += result_real[y][j] * sin(theta) + result_complex[y][j] * cos(theta);
            }
        }
    }

    for(int i = 0; i < height; ++i){ // y
        float num = height * width;
        for(int j = 0; j < width; ++j){ // x
            float theta = 0, value = 0;
            for(int x = 0; x < width; ++x){ // u
                theta = double_pi * x * j / width;
                value += temp_real[i][x] * cos(theta) - temp_complex[i][x] * sin(theta);
            }
            float temp = value / num;
            if(temp < 0) temp = -temp;
            ifft_img.at<uchar>(i, j) = 0.5f + temp;
        }
    }

    int64_t END_TIME = getTimeNow();
    printf("IFFT -> %f ms\n", (END_TIME - BEGIN_TIME) / 1000.0);

    for(int i = 0; i < height; ++i){
        delete[]temp_real[i];
        delete[]temp_complex[i];
    }
    delete[]temp_real;
    delete[]temp_complex;
    return ifft_img;
}

int main(){
    // initialize img
    Mat img = imread("../src/49.png", 0);
    findProperSize(img);
    makeImgInCenter(img);
    printf("img -> width: %d, height: %d\n", img.cols, img.rows);

    // FFT & IFFT
    fftPair trans(img);
    Mat fft_img = fft2d(&trans);
    imshow("fft", fft_img);
    waitKey();
    return 0;
}