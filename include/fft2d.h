/**
 * @file fft2d.h
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _FFT2D_H
#define _FFT2D_H

#include "fft.h"

/**
 * @brief The interface of FFT2d, call fft() to calculate two dimension FFT
 *        fftPair() includes Mat, float **real, float **complex
 * 
 * @param arg: fftPair
 * @return Mat: the image in frequency
 */
Mat fft2d(fftPair *arg);

#endif