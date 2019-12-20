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
    for(int i = 0; i < height; ++i)
        for(int j = 0; j < width; ++j)
            if((i + j) & 1) img.at<float>(i, j) = -img.at<float>(i, j);
}

/**
 * @brief make the img width and height be the 2^n(for FFT)
 * 
 * @param img 
 */
void findProperSize(Mat &img, int height, int width){
    if(height <= 0 || width <= 0){
        width = img.cols;
        height = img.rows;
        int ori_width = width;
        int ori_height = height;
        for(int i = 1; i <= 16; i <<= 1){
            width |= width >> i;
            height |= height >> i;
        }
        width += 1, height += 1;
        if(ori_width << 1 == width) width = ori_width;
        if(ori_height << 1 == height) height = ori_height;
        resize(img, img, Size(width, height));
    }else{
        resize(img, img, Size(width, height));
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
Mat generateFrequencyImg(fftPair *arg){
    int width = arg->img.cols;
    int height = arg->img.rows;
    Mat res(height, width, CV_8UC1, Scalar(0));
    float max = 0;
    float **length = new float*[height];
    for(int i = 0; i < height; ++i){
        length[i] = new float[width];
        memset(length[i], 0, sizeof(float) * width);
    }
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            length[i][j] = log(1.0f + sqrt(pow(arg->result_real[j][i], 2) + pow(arg->result_complex[j][i], 2)));
            if(max < length[i][j]) max = length[i][j];
        }
    }
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            res.at<uchar>(i, j) = 0.5f + length[i][j] * 255 / max;
        }
    }
    for(int i = 0; i < height; ++i){
        delete[]length[i];
    }
    delete[]length;
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
void calculateW(int height, int width, float *width_W_real, float *width_W_complex, float *height_W_real, float *height_W_complex, float sign){
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
void invertSign(int height, int width, float **matrix){
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            matrix[i][j] = -matrix[i][j];
        }
    }
}

/**
 * @brief Get the Rand Sequence object
 * 
 * @param arr 
 * @param low 
 * @param high 
 */
void getRandSequence(vector<float> &vec, int low, int high){
    srand(time(NULL));
    int count = high - low;
    int tail = high - 1;
    for(int i = 0; i < count; ++i){
        int idx = rand() % (count - i) + low;
        float temp = 0;
        temp = vec[idx];
        vec[idx] = vec[tail];
        vec[tail] = temp;
        --tail;
    }
}
