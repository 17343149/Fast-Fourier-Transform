/**
 * @file ifft2d.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "ifft2d.h"

Mat ifft2d(fftPair *arg){
    int width = arg->img.cols, height = arg->img.rows;

    // calculate W
    float *width_W_real = new float[width]; // the W of row
    float *width_W_complex = new float[width];
    float *height_W_real = new float[height]; // the W of column
    float *height_W_complex = new float[height];
    calculateW(height, width, width_W_real, width_W_complex, height_W_real, height_W_complex, -1.0f);

    // middle matrix
    float **mid_real = new float*[height];
    float **mid_complex = new float*[height];
    for(int i = 0; i < height; ++i){
        mid_real[i] = new float[width];
        mid_complex[i] = new float[width];
        memset(mid_real[i], 0, sizeof(float) * width);
        memset(mid_complex[i], 0, sizeof(float) * width);
    }

    // result matrix
    float **res_real = new float*[width];
    float **res_complex = new float*[width];
    for(int i = 0; i < width; ++i){
        res_real[i] = new float[height];
        res_complex[i] = new float[height];
        memset(res_real[i], 0, sizeof(float) * height);
        memset(res_complex[i], 0, sizeof(float) * height);
    }

    // calculate passed time
    int64_t BEGIN_TIME = getTimeNow();

    // complex conjugate
    invertSign(width, height, arg->result_complex);

    // transform in row!
    for(int i = 0; i < height; ++i){
        fft(arg->img, i, -1, 0, width, 1, arg->result_real, arg->result_complex, mid_real[i], mid_complex[i], width_W_real, width_W_complex, height_W_real, height_W_complex, true);
    }

    // transform in column!
    for(int i = 0; i < width; ++i){
        fft(arg->img, -1, i, 0, height, 1, mid_real, mid_complex, res_real[i], res_complex[i], width_W_real, width_W_complex, height_W_real, height_W_complex, true);
    }

    // generate image
    Mat res(height, width, CV_8UC1, Scalar(0));
    float sum = width * height;
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            float pixel = res_real[j][i] / sum;
            if(pixel < 0) pixel = -pixel;
            res.at<uchar>(i, j) = 0.5f + pixel;
        }
    }

    // calculate passed time
    int64_t END_TIME = getTimeNow();
    printf("ifft2d: %f s (%ld ms)\n", (END_TIME - BEGIN_TIME) / 1000.0f, END_TIME - BEGIN_TIME);
    
    // to recover matrix in fftPair, complex conjugate again
    invertSign(width, height, arg->result_complex);

    // delete array
    for(int i = 0; i < height; ++i){
        delete[]res_real[i];
        delete[]res_complex[i];
    }
    for(int i = 0; i < height; ++i){
        delete[]mid_real[i];
        delete[]mid_complex[i];
    }
    delete[]mid_real;
    delete[]mid_complex;
    delete[]res_real;
    delete[]res_complex;
    delete[]width_W_real;
    delete[]width_W_complex;
    delete[]height_W_real;
    delete[]height_W_complex;

    return res;
}