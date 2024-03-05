#include <iostream>

#include "Convolution.h"

// Default constructor
Convolution::Convolution() : customKernel(nullptr), kh(0), kw(0), stride_val(1), pad(true) {}


// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kernelHeight, int kernelWidth, int stride, bool padding){
    this->kh = kernelHeight;
    this->kw = kernelWidth;
    this->stride_val = stride;
    this->pad = padding;

    this->customKernel = new double*[kh];
    for (int i = 0; i < kh; ++i) {
        this->customKernel[i] = new double[kw];
        for (int j = 0; j < kw; ++j) {
            this->customKernel[i][j] = customKernel[i][j];
        }
    }

}

// Destructor
Convolution::~Convolution() {
    if (customKernel != nullptr) {
        for (int i = 0; i < kh; i++) {
            delete[] customKernel[i];

        }
        delete[] customKernel;
    }
    customKernel = nullptr;
}

// Copy constructor
Convolution::Convolution(const Convolution &other){

    kh = other.kh;
    kw = other.kw;
    stride_val = other.stride_val;
    pad = other.pad;
    customKernel = new double*[kh];
    for (int i = 0; i < kh; i++) {
        customKernel[i] = new double[kw];
        for (int j = 0; j < kw; j++) {
            customKernel[i][j] = other.customKernel[i][j];

        }
    }

}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {

    if (this != &other) {
        for (int i = 0; i < kh; i++) {
            delete[] customKernel[i];
        }
        delete[] customKernel;


        kh = other.kh;
        kw = other.kw;
        stride_val = other.stride_val;
        pad = other.pad;

        customKernel = new double*[kh];
        for (int i = 0; i < kh; i++) {
            customKernel[i] = new double[kw];
            for (int j = 0; j < kw; j++) {
                customKernel[i][j] = other.customKernel[i][j];

            }
        }
    }

    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix &input_image) const {

    int input_height = input_image.get_height();
    int input_width = input_image.get_width();

    int padded_height = input_height;
    int padded_width = input_width;

    int convolved_matrix_height = (input_height-kh+2*pad)/stride_val +1;
    int convolved_matrix_width = (input_width-kw+2*pad)/stride_val +1;

    if (pad) {
        padded_height = input_height + 2;
        padded_width = input_width + 2;
    }

    ImageMatrix padded_image(padded_height, padded_width);

    if (pad) {
        for (int i = 0; i < input_height; i++) {
            for (int j = 0; j < input_width; j++) {
                padded_image.set_pixel(input_image.get_data(i, j), i + 1, j + 1);
            }
        }
    } else {
        padded_image = input_image;
    }



    ImageMatrix convolved_matrix(convolved_matrix_height, convolved_matrix_width);

    int i = 0;
    int a = 0;
    while (i + kh <= padded_height){
        int j = 0;
        int b = 0;
        while (j + kw <= padded_width){
            double result = 0;
            for (int m = 0; m < kh; m++) {
                for (int n = 0; n < kw; n++) {
                    if (i + m < padded_height && j + n < padded_width) {
                        result += padded_image.get_data(i+ m, j + n) * customKernel[m][n];
                    }
                }
            }
            convolved_matrix.set_pixel(result, a , b );

            j += stride_val;
            b ++;
        }
        i += stride_val;
        a++;
    }

    return convolved_matrix;


}
