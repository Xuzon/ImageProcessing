#include "ImageProcessor.h"

void ImageProcessor::UpsideDown() {
    for (int y = 0, i = 0; y < H / 2; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            uchar r = pixR[i];
            uchar g = pixG[i];
            uchar b = pixB[i];

            int pos = (H - y - 1) * S + x * 3;

            pixR[i] = pixR[pos];
            pixG[i] = pixG[pos];
            pixB[i] = pixB[pos];

            pixR[pos] = r;
            pixG[pos] = g;
            pixB[pos] = b;
        }
    }
}

void ImageProcessor::Invert() {
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            pixR[i] = 255 - pixR[i];
            pixG[i] = 255 - pixG[i];
            pixB[i] = 255 - pixB[i];
        }
    }
}

void ImageProcessor::GrayScale() {
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int average = pixR[i] + pixG[i] + pixB[i];
            average /= 3;
            pixR[i] = average;
            pixG[i] = average;
            pixB[i] = average;
        }
    }
}

void ImageProcessor::OldFilter() {
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int average = pixR[i] + pixG[i] + pixB[i];
            average /= 3;
            pixR[i] = average + rOld > 255 ? 255 : average + rOld;
            pixG[i] = average + gOld > 255 ? 255 : average + gOld;
            pixB[i] = average + bOld > 255 ? 255 : average + bOld;

        }
    }
}

ImageProcessor::ImageProcessor(uchar* pixR, uchar* pixG, uchar* pixB, int W, int H, int Padding, int S) {

    this->pixR = pixR;
    this->pixG = pixG;
    this->pixB = pixB;
    this->W = W;
    this->H = H;
    this->Padding = Padding;
    this->S = S;
}

ImageProcessor::~ImageProcessor() {
}
