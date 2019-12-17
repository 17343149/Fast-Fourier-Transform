# Fast-Fourier-Transform

- Date: 2019/12/17
- Version: 1.0.0

## Description

FFT2d && IFFT2d

## Details

- **FFT**  
![](asset/figure0.png)  
![](asset/figure1.png)  
![](asset/figure2.png)  
![](asset/figure3.png)  

- **IFFT**  
![](asset/ifft_en.png)  
![](asset/ifft_ch.png)


## Run time

- test file: [src/49.png](src/49.png), to perform FFT & IFFT, it resize(512 x 512), **one channel, grey image** 
![](asset/time.png)  
![](src/49.png)  
![](asset/fft.png)  
![](asset/ifft.png)

## How to run
```shell
mkdir build
cd build
cmake ..
make
./main
```
