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
    float **res_real[3];
    float **res_complex[3];
    for(int k = 0; k < 3; ++k){
        res_real[k] = new float*[width];
        res_complex[k] = new float*[width];
        for(int i = 0; i < width; ++i){
            res_real[k][i] = new float[height];
            res_complex[k][i] = new float[height];
            memset(res_real[k][i], 0, sizeof(float) * height);
            memset(res_complex[k][i], 0, sizeof(float) * height);
        }
    }

    // generate image
    Mat res(height, width, CV_32FC3, Scalar(0));
    float sum = width * height;

    // calculate passed time
    int64_t BEGIN_TIME = getTimeNow();

    for(int k = 0; k < 3; ++k){
        // complex conjugate
        invertSign(width, height, arg->result_complex[k]);

        // transform in row!
        for(int i = 0; i < height; ++i){
            fft(arg->img, k, i, -1, 0, width, 1, arg->result_real[k], arg->result_complex[k], mid_real[i], mid_complex[i], width_W_real, width_W_complex, height_W_real, height_W_complex, true);
        }

        // transform in column!
        for(int i = 0; i < width; ++i){
            fft(arg->img, k, -1, i, 0, height, 1, mid_real, mid_complex, res_real[k][i], res_complex[k][i], width_W_real, width_W_complex, height_W_real, height_W_complex, true);
        }

        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                res.at<Vec3f>(i, j)[k] = res_real[k][j][i] / sum;
            }
        }
        invertSign(width, height, arg->result_complex[k]);
    }

    // calculate passed time
    int64_t END_TIME = getTimeNow();
    printf("ifft2d: %f s (%ld ms)\n", (END_TIME - BEGIN_TIME) / 1000.0f, END_TIME - BEGIN_TIME);

    // delete array
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < width; ++i){
            delete[]res_real[k][i];
            delete[]res_complex[k][i];
        }
        delete[]res_real[k];
        delete[]res_complex[k];
    }
    for(int i = 0; i < height; ++i){
        delete[]mid_real[i];
        delete[]mid_complex[i];
    }
    delete[]mid_real;
    delete[]mid_complex;
    delete[]width_W_real;
    delete[]width_W_complex;
    delete[]height_W_real;
    delete[]height_W_complex;

    return res;
}