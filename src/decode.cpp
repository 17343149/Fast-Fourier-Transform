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
                if(k == 0 && i == 0)
                    printf("(%d, %d, %d) -> %f - %f = %f\n", i, j, k, encode->result_real[k][j][i], origin->result_real[k][j][i], pixel);
            }
        }
    }

    imshow("middle", middle);

    int half_height = height / 2;
    Mat res(height, width, CV_32FC3, Scalar(0));
    for(int k = 0; k < 3; ++k){
        for(int i = 0; i < half_height; ++i){
            for(int j = 0; j < width; ++j){
                res.at<Vec3f>(vec[0][i], vec[1][j])[k] = middle.at<Vec3f>(i, j)[k];
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