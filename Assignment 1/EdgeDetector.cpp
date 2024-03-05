// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>
#include <iostream>

// Default constructor
EdgeDetector::EdgeDetector() {

}

// Destructor
EdgeDetector::~EdgeDetector() {

}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    const double* Gx[3] = {
            new double[3]{-1, 0, 1},
            new double[3]{-2, 0, 2},
            new double[3]{-1, 0, 1}
    };
    const double* Gy[3] = {
            new double[3]{-1, -2, -1},
            new double[3]{0, 0, 0},
            new double[3]{1, 2, 1}
    };
    Convolution convolutionX(const_cast<double **>(Gx), 3, 3, 1, true);
    ImageMatrix Ix = convolutionX.convolve(input_image);
    for (int i = 0; i < 3; i++) {
        delete[] Gx[i];
    }

    Convolution convolutionY(const_cast<double **>(Gy), 3, 3, 1, true);
    ImageMatrix Iy = convolutionY.convolve(input_image);



    for (int i = 0; i < 3; i++) {
        delete[] Gy[i];
    }


    int input_height = input_image.get_height();
    int input_width = input_image.get_width();

    ImageMatrix G_magnitude(input_height,input_width);

    for (int i = 0; i < input_height; i++) {
        for (int j = 0; j < input_width; j++) {
            double valueX =  Ix.get_data(i,j);
            double  valueY = Iy.get_data(i,j);
            G_magnitude.set_pixel(sqrt(pow(valueX, 2) + pow(valueY, 2)),i,j);
        }
    }


    double sum = 0;
    for (int i = 0; i < input_height; i++) {
        for (int j = 0; j < input_width; j++) {
            sum += G_magnitude.get_data(i,j);
        }
    }
    double threshold = sum / (input_height * input_width);


    std::vector<std::pair<int, int>> EdgePixels;
    for (int i = 0; i < input_height; i++) {
        for (int j = 0; j < input_width; j++) {
            if (G_magnitude.get_data(i,j) > threshold) {
                EdgePixels.push_back(std::make_pair(i, j));
            }
        }
    }

    return EdgePixels;
}

