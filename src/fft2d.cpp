/**
 * @file fft2d.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "fft2d.h"

Mat fft2d(fftPair *arg){
    int width = arg->img.cols, height = arg->img.rows;

    // calculate W
    float *width_W_real = new float[width]; // W of row
    float *width_W_complex = new float[width];
    float *height_W_real = new float[height]; // W of column
    float *height_W_complex = new float[height];
    calculateW(width, height, width_W_real, width_W_complex, height_W_real, height_W_complex);

    // middle matrix to save temporary real and complex values
    float **mid_real = new float*[height];
    float **mid_complex = new float*[height];
    float **length = new float*[height];
    for(int i = 0; i < height; ++i){
        mid_real[i] = new float[width];
        mid_complex[i] = new float[width];
        length[i] = new float[width];
        memset(mid_real[i], 0, sizeof(float) * width);
        memset(mid_complex[i], 0, sizeof(float) * width);
        memset(length[i], 0, sizeof(float) * width);
    }

    // calculate passed time
    int64_t BEGIN_TIME = getTimeNow();

    // transform in row!
    for(int i = 0; i < height; ++i){
        fft(arg->img, i, -1, 0, width, 1, nullptr, nullptr, mid_real[i], mid_complex[i], width_W_real, width_W_complex, height_W_real, height_W_complex);
    }

    // transform in column!
    for(int i = 0; i < width; ++i){
        fft(arg->img, -1, i, 0, height, 1, mid_real, mid_complex, arg->result_real[i], arg->result_complex[i], width_W_real, width_W_complex, height_W_real, height_W_complex);
    }

    // calculate passed time
    int64_t END_TIME = getTimeNow();
    printf("fft2d: %f s (%ld ms)\n", (END_TIME - BEGIN_TIME) / 1000.0f, END_TIME - BEGIN_TIME);

    // generate the image
    float max = 0;
    calculateLength(arg, length, max);
    Mat res = generateFrequencyImg(width, height, length, max);

    for(int i = 0; i < height; ++i){
        delete[]mid_real[i];
        delete[]mid_complex[i];
        delete[]length[i];
    }
    delete[]mid_real;
    delete[]mid_complex;
    delete[]length;
    delete[]width_W_real;
    delete[]width_W_complex;
    delete[]height_W_real;
    delete[]height_W_complex;

    return res;
}