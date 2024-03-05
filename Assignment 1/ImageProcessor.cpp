#include <iostream>
#include "ImageProcessor.h"


ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {

    ImageSharpening sharpening;
    ImageMatrix sharpedMatrix = sharpening.sharpen(img,2.0);

    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> EdgePixels = edgeDetector.detectEdges(sharpedMatrix);

    DecodeMessage decodeMessage;
    std::string messageP = decodeMessage.decodeFromImage(sharpedMatrix,EdgePixels);

    return messageP;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {

    ImageSharpening sharpening;
    ImageMatrix sharpedMatrix = sharpening.sharpen(img,2.0);

    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> EdgePixels = edgeDetector.detectEdges(sharpedMatrix);

    EncodeMessage encodeMessage;
    encodeMessage.encodeMessageToImage(img,message,EdgePixels);

    return img;


}
