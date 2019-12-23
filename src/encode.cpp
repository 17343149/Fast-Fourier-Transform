/**
 * @file encode.cpp
 * @author Haoxuan Zhang (zhanghx59@mail2.sysu.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2019-12-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "encode.h"

Mat getEncodeImg(Mat &img, fftPair *arg, vector<vector<float> > &vec){
    int height = arg->img.rows;
    int width = arg->img.cols;
    int half_height = height / 2;

    Mat temp(half_height, width, CV_32FC3, Scalar(0));
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < img.rows; ++i){
            for(int j = 0; j < img.cols; ++j){
                temp.at<Vec3f>(i, j)[k] = (float)img.at<Vec3b>(i, j)[k] / 255.0;
            }
        }
    }

    vector<float> M;
    vector<float> N;
    for(int i = 0; i < half_height; ++i){
        M.push_back(i);
    }
    for(int i = 0; i < width; ++i){
        N.push_back(i);
    }
    getRandSequence(M, 0, half_height);
    getRandSequence(N, 0, width);
    vec.push_back(M);
    vec.push_back(N);

    Mat res(height, width, CV_32FC3, Scalar(0));
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < half_height; ++i){
            for(int j = 0; j < width; ++j){
                res.at<Vec3f>(i, j)[k] = res.at<Vec3f>(height - 1 - i, width - 1 - j)[k] = temp.at<Vec3f>(M[i], N[j])[k];
            }
        }
    }

    return res;
}

void encode(fftPair *arg, Mat &img, float factor){
    int width = arg->img.cols;
    int height = arg->img.rows;
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){ 
                arg->result_real[k][j][i] += factor * img.at<Vec3f>(i, j)[k];
            }
        }
    }
}

// Interface
Mat encode(Mat ori_img, Mat water_img){
    int height = ori_img.rows;
    int width = ori_img.cols;
    fftPair encryption(ori_img);

    // process addition image
    vector<vector<float> > vec;
    resize(water_img, water_img, Size(ori_img.cols, ori_img.rows / 2));
    Mat addition_img = getEncodeImg(water_img, &encryption, vec);

    // FFT
    Mat fft_img = fft2d(&encryption);

    // encode!!!
    encode(&encryption, addition_img, 35.0f);

    Mat ifft_img = ifft2d(&encryption);
    resize(ifft_img, ifft_img, Size(width, height));
    if(ifft_img.type() != CV_8UC3){
        ifft_img *= 255;
        ifft_img.convertTo(ifft_img, CV_8UC3);
    }
    return ifft_img;
}