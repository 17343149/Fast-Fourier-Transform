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

static string test_img = "../src/49.png";
static string girlfriend = "../src/girlfriend.jpg";
static string addition = "../src/addition.jpg";

void TEST(Mat img){
    cout << img.type() << endl;
    for(int i = 0; i < img.rows; ++i){
        for(int j = 0; j < img.cols; ++j){
            unsigned int temp = img.at<uchar>(i, j);
            cout << i << ", " << j << ": " << temp << endl;
        }
    }
}

int main(){
    // initialize img
    Mat img = imread(girlfriend, 0);
    Mat water_img = imread(addition, 0);

    fftPair encryption(img);

    // process addition image
    // vector<vector<float> > vec;
    // Mat addition_img = getEncodeImg(water_img, &encryption, vec);
    // imshow("encode", addition_img);

    // FFT
    Mat fft_img = fft2d(&encryption);
    imshow("fft", fft_img);

    // encode
    // encode(&encryption, addition_img, 2.0f);

    // // IFFT
    // Mat ifft_img = ifft2d(&encryption);
    // imshow("ifft", ifft_img);

    // // decode
    // Mat res = decode(ifft_img, img, vec);
    // imshow("res", res);

    waitKey();
    return 0;
}