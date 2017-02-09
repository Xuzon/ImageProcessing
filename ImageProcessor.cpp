#include "ImageProcessor.h"

void ImageProcessor::ChangeBrightness(int value) {
    float value02 = (float)value / (float)50;
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int r = pixRCopy[i] * value02;
            int g = pixGCopy[i] * value02;
            int b = pixBCopy[i] * value02;
            pixR[i] = r > 255 ? 255 : r;
            pixG[i] = g > 255 ? 255 : g;
            pixB[i] = b > 255 ? 255 : b;
        }
    }
}

void ImageProcessor::ChangeContrast(int contrast, int brightness){
    float fContrast = (float)contrast / 50.0;
    float fBright = (((float)brightness / 50.0) - 1 ) * 255;
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int r = ContrastTransform(fContrast, fBright, pixRCopy[i]);
            int g = ContrastTransform(fContrast, fBright, pixGCopy[i]);
            int b = ContrastTransform(fContrast, fBright, pixBCopy[i]);
            pixR[i] = r;
            pixG[i] = g;
            pixB[i] = b;
        }
    }
}

int ImageProcessor::ContrastTransform(float contrast, float brightness, int value) {
    int toRet = value;
    toRet = contrast * (toRet - 128) + 128 + brightness;
    toRet = ImageProcessor::Clamp0255(toRet);
    return toRet;
}

int ImageProcessor::Clamp0255(int value) {
    int toRet = value;
    toRet = toRet > 255 ? 255 : toRet;
    toRet = toRet < 0 ? 0 : toRet;
    return toRet;
}

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

void ImageProcessor::SepiaFilter() {
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            uchar r = pixR[i];
            uchar g = pixG[i];
            uchar b = pixB[i];
            int outR = r * 0.393 + g * 0.769 + b * .189;
            int outG = r * 0.349 + g * 0.686 + b * .168;
            int outB = r * 0.272 + g * 0.534 + b * .131;
            pixR[i] = outR > 255 ? 255 : outR;
            pixG[i] = outG > 255 ? 255 : outG;
            pixB[i] = outB > 255 ? 255 : outB;

        }
    }
}

void ImageProcessor::SepiaFilter(uchar thresholdR, uchar thresholdG, uchar thresholdB,          uchar diffRG) {
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            uchar r = pixR[i];
            uchar g = pixG[i];
            uchar b = pixB[i];
            uchar diff = r - g;
            if (r < thresholdR || g < thresholdG || b < thresholdB || g > r || diff < diffRG) {
                int outR = r * 0.393 + g * 0.769 + b * .189;
                int outG = r * 0.349 + g * 0.686 + b * .168;
                int outB = r * 0.272 + g * 0.534 + b * .131;
                pixR[i] = outR > 255 ? 255 : outR;
                pixG[i] = outG > 255 ? 255 : outG;
                pixB[i] = outB > 255 ? 255 : outB;
            }
        }
    }
}

void ImageProcessor::RadialBlur(int centerX,int centerY) {
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            uchar r = pixR[i];
            uchar g = pixG[i];
            uchar b = pixB[i];
            int outR = r * 0.393 + g * 0.769 + b * .189;
            int outG = r * 0.349 + g * 0.686 + b * .168;
            int outB = r * 0.272 + g * 0.534 + b * .131;
            pixR[i] = outR > 255 ? 255 : outR;
            pixG[i] = outG > 255 ? 255 : outG;
            pixB[i] = outB > 255 ? 255 : outB;

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
    this->copyImage = new uchar[S * H];
    memcpy(this->copyImage, this->pixR,H * S);
    pixBCopy = (pixGCopy = (pixRCopy = copyImage) + 1) + 1;
}

ImageProcessor::~ImageProcessor() {
    if (copyImage != nullptr) {
        delete[] copyImage;
    }
}
