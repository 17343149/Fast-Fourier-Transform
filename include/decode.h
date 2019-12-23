/**
 * @file decode.h
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _DECODE_H
#define _DECODE_H

#include "fft2d.h"

/**
 * @brief Get the Water Img object
 * 
 * @param arg 
 * @param vec 
 * @return Mat 
 */
Mat getDecodeImg(fftPair *encode, fftPair *origin, vector<vector<float> > &vec);

/**
 * @brief Interface of decode
 * 
 * @param img 
 * @param ori_img 
 * @return Mat 
 */
Mat decode(Mat img, Mat ori_img);

#endif