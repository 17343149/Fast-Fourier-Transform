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
#include "encode.h"
#include "decode.h"

static string TEST_IMG = "../test_img/test1.jpg";
static string WATER_IMG = "../test_img/water1.jpg";
static string RESULT_DIR = "../res/";

int main(){
    // initialize img
    Mat img = imread(TEST_IMG);
    Mat water_img = imread(WATER_IMG);

    Mat processed_img = encode(img, water_img);

    // Mat processed_img = imread("../res/processed_test3.jpg");

    Mat res_img = decode(processed_img, img);

    imshow("processed", processed_img);
    imshow("res", res_img);
    waitKey();
    return 0;
}