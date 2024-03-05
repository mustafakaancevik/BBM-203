#ifndef ENCODE_MESSAGE_H
#define ENCODE_MESSAGE_H

#include <string>
#include <vector>
#include "ImageMatrix.h"

class EncodeMessage {
public:
    EncodeMessage();
    ~EncodeMessage();

    ImageMatrix encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions);
    static bool isPrime(int n);
    static int fibonacci(int n);
    static std::string transformMessage(const std::string &message);
    static std::string circularShift(const std::string& message);
    static std::string asciiToBinary(const std::string& input);
private:
    // Any private helper functions or variables if necessary

    
};

#endif // ENCODE_MESSAGE_H
