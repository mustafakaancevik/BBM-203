#include "EncodeMessage.h"
#include "ImageMatrix.h"
#include <cmath>
#include <iostream>
#include <bitset>
#include <cstdint>


// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {

    std::string binary_string = transformMessage(message);

    int message_position = 0;

    ImageMatrix encodedImage = img;

    for (const auto& pixel : positions) {
        int x = pixel.first;
        int y = pixel.second;
        if (x <= img.get_height() && y <= img.get_width()){
            if (message_position >= binary_string.length()) {
                break;
            }

            int pixelValue = encodedImage.get_data(x, y);

            if (binary_string[message_position] == '0') {
                pixelValue = pixelValue & 0xFE;
            } else if (binary_string[message_position] == '1') {
                pixelValue = pixelValue | 0x01;
            }


            encodedImage.set_pixel(pixelValue, x, y);

            message_position++;
        }


    }

    return encodedImage;

}

bool EncodeMessage::isPrime(int n) {
    if (n <= 1)
        return false;

    for (int i = 2; i < n ; i++)
        if (n % i == 0)
            return false;

    return true;
}


std::string EncodeMessage::transformMessage(const std::string &message) {
    std::string transformed_message = message;

    for (int i = 0; i < message.size(); ++i) {

        if(isPrime(i)){
            int value = static_cast<int>(message[i]) + fibonacci(i);
            if (value <= 32) {
                value += 33;
            } else if (value >= 127) {
                value = 126;
            }
            transformed_message[i] = static_cast<char>(value);

        }

    }
    transformed_message = circularShift(transformed_message);

    return asciiToBinary(transformed_message);
}

int EncodeMessage::fibonacci(int n) {
    int a = 0, b = 1;
    int result = 0;

    if (n == 0) {
        return a;
    } else if (n==1){
        return 1;
    }

    for (int i = 2; i <= n; i++) {
        result = a + b;
        a = b;
        b = result;
    }

    return b;}

std::string EncodeMessage::circularShift(const std::string &message) {
    int shift_amount = message.length() / 2;
    return message.substr(message.length() - shift_amount) + message.substr(0, message.length() - shift_amount);
}

std::string EncodeMessage::asciiToBinary(const std::string &input) {
    std::string binaryString;

    for (char c : input) {
        std::bitset<7> binaryChar(static_cast<char>(c));
        binaryString += binaryChar.to_string();
    }

    return binaryString;
}







