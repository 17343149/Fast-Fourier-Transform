/**
 * @file encode.h
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _ENCODE_H
#define _ENCODE_H

#include "fft2d.h"

/**
 * @brief Get the Encode Img object
 * 
 * @param img 
 * @param arg 
 * @param vec 
 * @return Mat 
 */
Mat getEncodeImg(Mat &img, fftPair *arg, vector<vector<float> > &vec);

/**
 * @brief 
 * 
 * @param arg 
 * @param factor 
 */
void encode(fftPair*, Mat &arg, float factor = 1.0f);

#endif