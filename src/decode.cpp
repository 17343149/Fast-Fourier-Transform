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
    Mat middle(height, width, CV_32FC3, Scalar(0));
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                float pixel = encode->result_real[k][j][i] - origin->result_real[k][j][i];
                if(pixel <= 0) middle.at<Vec3f>(i, j)[k] = 0;
                else if(pixel >= 1) middle.at<Vec3f>(i, j)[k] = 1;
                else middle.at<Vec3f>(i, j)[k] = pixel;
            }
        }
    }

    int half_height = height / 2;
    Mat res(height, width, CV_32FC3, Scalar(0));
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < half_height; ++i){
            for(int j = 0; j < width; ++j){
                res.at<Vec3f>(vec[0][i], vec[1][j])[k] = middle.at<Vec3f>(i, j)[k];
            }
        }
    }

    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < half_height; ++i){
            for(int j = 0; j < width; ++j){
                res.at<Vec3f>(height - 1 - i, width - 1 - j)[k] = res.at<Vec3f>(i, j)[k];
            }
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

// Interface
Mat decode(Mat img, Mat ori_img){
    int ori_height = img.rows;
    int ori_width = img.cols;

    fftPair encode(img);
    fftPair origin(ori_img);
    fft2d(&encode);
    fft2d(&origin);

    int height = encode.img.rows;
    int width = encode.img.cols;
    int half_height = height / 2;
    vector<vector<float> > vec;
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

    Mat res = getDecodeImg(&encode, &origin, vec);
    resize(res, res, Size(ori_width, ori_height));
    if(res.type() != CV_8UC3){
        res *= 255;
        res.convertTo(res, CV_8UC3);
    }
    return res;
}