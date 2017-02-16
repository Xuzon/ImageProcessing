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
    ContrastTransform(fContrast, fBright);
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int r = pixRCopy[i];
            int g = pixGCopy[i];
            int b = pixBCopy[i];
            pixR[i] = LUT[r];
            pixG[i] = LUT[g];
            pixB[i] = LUT[b];
        }
    }
}

void ImageProcessor::ContrastTransform(float contrast, float brightness) {
    int val = 0;
    for (int i = 0; i < 256; i++) {
        val = contrast * (i - 128) + 128 + brightness;
        LUT[i] = ImageProcessor::Clamp0255(val);
    }
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

///
///Show in white (255,255,255) the edges
///
void ImageProcessor::Edges(float threshold,EdgeMetric metric) {
    float calculatedThreshold = threshold;
    if (metric == EdgeMetric::EuclideanDistance) {
        calculatedThreshold *= calculatedThreshold;
    }
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int r = pixRCopy[i];
            int g = pixGCopy[i];
            int b = pixBCopy[i];
            Vector3* pixel = new Vector3(r, g, b);
            r = g = b = 0;
            int count = 0;
            Vector3* nearbyPixels = GetNearbyPixels(i, &count,x);
            for (int j = 0; j < count; j++) {
                float edge = 0;

                switch (metric) {
                case EdgeMetric::AbsDifference:
                    edge = Vector3::AbsDifference(pixel, &nearbyPixels[j]);
                    break;
                case EdgeMetric::MaxDifference:
                    edge = Vector3::MaxDifference(pixel, &nearbyPixels[j]);
                    break;
                case EdgeMetric::EuclideanDistance:
                    edge = Vector3::SqrDistance(pixel, &nearbyPixels[j]);
                    break;
                }
                if (edge > calculatedThreshold) {
                    r = g = b = 255;
                    break;
                }
            }
            delete[] nearbyPixels;
            delete pixel;
            pixR[i] = r;
            pixG[i] = g;
            pixB[i] = b;
        }
    }
}

Vector3* ImageProcessor::GetNearbyPixels(int i, int* count,int x) {
    int* pos = new int[8];
    *count = 0;
    //upper positions
    pos[0] = i - S - 3;
    pos[1] = i - S;
    pos[2] = i - S + 3;
    //middle positions
    pos[3] = i - 3;
    pos[4] = i + 3;
    //lower positions
    pos[5] = i + S - 3;
    pos[6] = i + S;
    pos[7] = i + S + 3;

    //Crop the image at left
    if (x == 0) {
        pos[0] = pos[3] = pos[5] = -1;
    }

    //Crop the image at right
    if (x == (W - 1)) {
        pos[2] = pos[4] = pos[7] = -1;
    }

    //Crop the image up and down
    for (int j = 0; j < 8; j++) {
        if (pos[j] > -1 && pos[j] < S * H) {
            (*count)++;
        } else {
            pos[j] = -1;
        }
    }
    //allocate array
    Vector3* vecs = new Vector3[*count];
    int k = 0;
    //asign
    for (int j = 0; j < 8; j++) {
        int currPos = pos[j];
        if (currPos > -1) {
            vecs[k].x = pixRCopy[currPos];
            vecs[k].y = pixGCopy[currPos];
            vecs[k].z = pixBCopy[currPos];
            k++;
        }
    }
    delete[] pos;
    return vecs;
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
