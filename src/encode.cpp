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

    Mat temp(half_height, width, CV_8UC3, Scalar(0));
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < img.rows; ++i){
            for(int j = 0; j < img.cols; ++j){
                temp.at<Vec3b>(i, j)[k] = img.at<Vec3b>(i, j)[k];
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

    Mat res(height, width, CV_8UC3, Scalar(0));
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < half_height; ++i){
            for(int j = 0; j < width; ++j){
                res.at<Vec3b>(i, j)[k] = res.at<Vec3b>(height - 1 - i, width - 1 - j)[k] = temp.at<Vec3b>(M[i], N[j])[k];
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
                arg->result_real[k][j][i] += factor * img.at<Vec3b>(i, j)[k];
            }
        }
    }
}