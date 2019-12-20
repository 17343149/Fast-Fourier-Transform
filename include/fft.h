/**
 * @file fft.h
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _FFT_H
#define _FFT_H

#include "common.h"

/**
 * @brief A function to calculate FFT or IFFT.
 *        In fact, it's one dimension FFT, for example, a row or a column
 * 
 * @param img: origin image
 * @param channel: img channel(B, G, R)
 * @param row: IF row > 0 THEN transform row ELSE transform column
 * @param col: choose the column to transform
 * @param idx: the index of begin of array, the value is 0 at the beginning 
 * @param K: the K in fourier transform, related to img row and column size 
 * @param step_scale: the scale of one step, for example, 1, 3, 5, 7, step_scale = 2
 * @param mid_real: middle values of matrix in frequency
 * @param mid_complex: same to mid_real
 * @param res_real: result matrix and used to backtrace
 * @param res_complex: same to arr_real
 * @param width_W_real: the value of W in the direction of row
 * @param width_W_complex 
 * @param height_W_real: the value of W in the direction of column
 * @param height_W_complex 
 * @param is_ifft: if used to calculate IFFT, default is false
 */
void fft(const Mat &img, int channel, int row, int col, int idx, int K, int step_scale, 
            float **mid_real, float **mid_complex, 
            float *res_real, float *res_complex, 
            float *width_W_real, float *width_W_complex, 
            float *height_W_real, float *height_W_complex, 
            bool is_ifft = false);

#endif