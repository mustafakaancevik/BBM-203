// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>
#include <valarray>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}
std::string DecodeMessage::binaryToASCII(const std::string& binaryString) {
    std::string ascii_string;
    std::string padded_binary_string = padBinaryString(binaryString);

    for (size_t i = 0; i < padded_binary_string.length(); i += 7) {
        std::string segment = padded_binary_string.substr(i, 7);
        int decimalValue = std::stoi(segment, 0, 2);

        if (decimalValue <= 32) {
            decimalValue += 33;
        }
        if (decimalValue >= 127) {
            decimalValue = 126;
        }

        ascii_string += static_cast<char>(decimalValue);
    }

    return ascii_string;
}

std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string binary_string;

    for (const auto& pixel : edgePixels) {
        int x = pixel.first;
        int y = pixel.second;
        int pixelValue = image.get_data(x, y);

        int lsb = pixelValue & 1;

        binary_string += std::to_string(lsb);
    }
    return binaryToASCII(binary_string);

}
std::string DecodeMessage::padBinaryString(const std::string& binaryString) {
    int zeros = (binaryString.length() % 7 > 0) ? 7 - (binaryString.length() % 7) : 0;

    std::string padded_binary_string = std::string(zeros, '0') + binaryString;
    return padded_binary_string;
}
