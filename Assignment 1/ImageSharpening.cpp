#include <iostream>
#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {

}

ImageSharpening::~ImageSharpening(){

}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    ImageMatrix sharpImage(input_image.get_height(),input_image.get_width());

    int height = input_image.get_height();
    int widht = input_image.get_width();

    const double* blurring_kernel[3] = {
            new double[3]{1.0/9.0, 1.0/9.0, 1.0/9.0},
            new double[3]{1.0/9.0, 1.0/9.0, 1.0/9.0},
            new double[3]{1.0/9.0, 1.0/9.0, 1.0/9.0}
    };

    Convolution convolution(const_cast<double **>(blurring_kernel), 3, 3, 1, true);

    ImageMatrix blurredImage = convolution.convolve(input_image);



    for (int i = 0; i < height ; i++) {
        for (int j = 0; j < widht; j++) {

            double input_image_data = input_image.get_data(i,j);
            double blurred_image_data = blurredImage.get_data(i,j);

            if (0 > input_image_data + k * (input_image_data - blurred_image_data )){
                sharpImage.set_pixel(0,i,j);
            } else if ( 255 < input_image_data + k * (input_image_data - blurred_image_data)){
                sharpImage.set_pixel(255,i,j);
            } else{
                sharpImage.set_pixel(input_image_data + k * (input_image_data - blurred_image_data),i,j);
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        delete[] blurring_kernel[i];
    }

    return sharpImage;


}
