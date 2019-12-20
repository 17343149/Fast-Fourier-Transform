/**
 * @file decode.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "decode.h"
#include "ifft2d.h"

Mat getDecodeImg(fftPair *encode, fftPair *origin, vector<vector<float> > &vec){
    int width = encode->img.cols;
    int height = encode->img.rows;
    Mat middle(height, width, CV_8UC1, Scalar(0));
    float max = 0, min = 0;
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            float pixel = encode->result_real[j][i] - origin->result_real[j][i];
            if(pixel <= 0) middle.at<uchar>(i, j) = 0;
            else if(pixel >= 255) middle.at<uchar>(i, j) = 255;
            else middle.at<uchar>(i, j) = pixel;
        }
    }

    imshow("middle", middle);

    int half_height = height / 2;
    Mat res(height, width, CV_8UC1, Scalar(0));
    for(int i = 0; i < half_height; ++i){
        for(int j = 0; j < width; ++j){
            res.at<uchar>(vec[0][i], vec[1][j]) = middle.at<uchar>(i, j);
        }
    }

    return res;
}

Mat decode(Mat &ifft_img, Mat &ori_img, vector<vector<float> > &vec){
    fftPair origin(ori_img);
    fftPair encode(ifft_img);
    fft2d(&origin);
    fft2d(&encode);

    Mat res = getDecodeImg(&encode, &origin, vec);

    return res;
}