/**
 * @file fft.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "fft.h"

void fft(const Mat &img, int channel, int row, int col, int idx, int K, int step_scale, 
            float **mid_real, float **mid_complex, 
            float *res_real, float *res_complex, 
            float *width_W_real, float *width_W_complex, 
            float *height_W_real, float *height_W_complex, 
            bool is_ifft) {

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
        fft(img, channel, row, col, idx, K, next_step_scale, mid_real, mid_complex, even_real, even_complex, width_W_real, width_W_complex, height_W_real, height_W_complex, is_ifft);
        
        // odd
        fft(img, channel, row, col, idx + step_scale, K, next_step_scale, mid_real, mid_complex, odd_real, odd_complex, width_W_real, width_W_complex, height_W_real, height_W_complex, is_ifft);

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
            res_real[i] = even_real[i] + temp_real;
            res_complex[i] = even_complex[i] + temp_complex;
            res_real[i + K] = even_real[i] - temp_real;
            res_complex[i + K] = even_complex[i] - temp_complex;
        }
        delete[]even_real;
        delete[]even_complex;
        delete[]odd_real;
        delete[]odd_complex;
    } else{
        if(is_ifft){ // if used to calculate ifft
            // decide transform in row or column
            res_real[0] = row >= 0? mid_real[idx][row]: mid_real[idx][col];
            res_complex[0] = row >= 0? mid_complex[idx][row]: mid_complex[idx][col];
        }else{
            res_real[0] = row >= 0? img.at<float>(row, idx): mid_real[idx][col];
            res_complex[0] = row >= 0? 0: mid_complex[idx][col];
        }
    }
}