/**
 * @file main.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "common.h"
#include "fft2d.h"
#include "ifft2d.h"

int main(){
    // initialize img
    Mat img = imread("../src/49.png", 0);

    // FFT & IFFT
    fftPair trans(img);
    Mat fft_img = fft2d(&trans);
    imshow("fft", fft_img);
    waitKey(1000);
    Mat ifft_img = ifft2d(&trans);
    imshow("ifft", ifft_img);
    waitKey(1000);
    return 0;
}