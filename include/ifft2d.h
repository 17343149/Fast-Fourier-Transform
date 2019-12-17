/**
 * @file ifft2d.h
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef _IFFT2D_H
#define _IFFT2D_H

#include "fft.h"

/**
 * @brief Interface of IFFT
 *        f*(x, y) = fft2d(F*(u, v)) / (width * height)
 * 
 * @param arg 
 * @return Mat 
 */
Mat ifft2d(fftPair *arg);

#endif



