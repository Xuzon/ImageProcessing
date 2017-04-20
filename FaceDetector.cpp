#include "ImageProcessor.h"

void FaceDetector::AddPixel(uchar r, uchar g, uchar b) {
    count++;
    CalculateAverage(r,g,b);
    CalculateTypicalDesviation(r,g,b);
}

void FaceDetector::DetectSkin() {
    for (int y = 0, i = 0; y < processor->H; y++, i += processor->Padding) {
        for (int x = 0; x < processor->W; x++, i += 3) {
            bool skin = true;
            if (processor->pixRCopy[i] > average.x + typicalDesviation.x || processor->pixRCopy[i] < average.x - typicalDesviation.x) {
                skin = false;
            }
            if (processor->pixGCopy[i] > average.y + typicalDesviation.y || processor->pixGCopy[i] < average.y - typicalDesviation.y) {
                skin = false;
            }
            if (processor->pixBCopy[i] > average.z + typicalDesviation.z || processor->pixBCopy[i] < average.z - typicalDesviation.z) {
                skin = false;
            }
            uchar toSet = skin ? 255 : 0;
            processor->pixR[i] = toSet;
            processor->pixG[i] = toSet;
            processor->pixB[i] = toSet;
        }
    }
}

void FaceDetector::DetectSkin(float rDesvMultiplier, float gDesvMultiplier, float bDesvMultiplier) {
    for (int y = 0, i = 0; y < processor->H; y++, i += processor->Padding) {
        for (int x = 0; x < processor->W; x++, i += 3) {
            bool skin = true;
            /*BOX DELIMITER
            if (processor->pixRCopy[i] > average.x + rDesvMultiplier * typicalDesviation.x || processor->pixRCopy[i] < average.x - rDesvMultiplier * typicalDesviation.x) {
                skin = false;
            }
            if (processor->pixGCopy[i] > average.y + gDesvMultiplier * typicalDesviation.y || processor->pixGCopy[i] < average.y - gDesvMultiplier * typicalDesviation.y) {
                skin = false;
            }
            if (processor->pixBCopy[i] > average.z + bDesvMultiplier * typicalDesviation.z || processor->pixBCopy[i] < average.z - bDesvMultiplier * typicalDesviation.z) {
                skin = false;
            }*/
            float r = (processor->pixL[i] - average.x);
            r *= r;
            float rDesv = typicalDesviation.x * rDesvMultiplier;
            r /= (rDesv * rDesv);
            float g = (processor->pixH[i] - average.y);
            g *= g;
            float gDesv = typicalDesviation.y * gDesvMultiplier;
            g /= (gDesv * gDesv);
            float b = (processor->pixS[i] - average.z);
            b *= b;
            float bDesv = typicalDesviation.z * bDesvMultiplier;
            b /= (bDesv * bDesv);
            if (r + g + b > 1) {
                skin = false;
            }
            processor->faceMask[i / 3] = skin ? 255 : 0;
        }
    }

    this->ApplyMask();
}

void FaceDetector::ApplyMask() {
    for (int y = 0, i = 0; y < processor->H; y++, i += processor->Padding) {
        for (int x = 0; x < processor->W; x++, i += 3) {
            bool skin = processor->faceMask[i / 3] == 255 ? true : false;
            processor->pixR[i] = skin ? processor->pixRCopy[i] : 0;
            processor->pixG[i] = skin ? processor->pixGCopy[i] : 0;
            processor->pixB[i] = skin ? processor->pixBCopy[i] : 0;
        }
    }
}

void FaceDetector::Dilate(int kernelSize) {
    GenerateMask(kernelSize);
    for (int y = 0; y < processor->H; y++) {
        for (int x = 0; x < processor->W; x++) {
            //set the copy value
            this->processor->faceMaskBackup[y * processor->W + x] = this->processor->faceMask[y * processor->W + x];
            //if my facemask has a 0
            if (this->processor->faceMask[y * processor->W + x] == 0){
                //check if I have to dilate
                if (AnyPixelInMask(x, y, kernelSize)) {
                    //dilate the copy
                    this->processor->faceMaskBackup[y * processor->W + x] = 255;
                }
            } 
        }
    }
    memcpy(this->processor->faceMask, this->processor->faceMaskBackup,this->processor->S * this->processor->H / 3);
    ApplyMask();
}

void FaceDetector::Erode(int kernelSize) {
    GenerateMask(kernelSize);
    for (int y = 0; y < processor->H; y++) {
        for (int x = 0; x < processor->W; x++) {
            //set the copy value
            this->processor->faceMaskBackup[y * processor->W + x] = this->processor->faceMask[y * processor->W + x];
            //if my facemask has a 0
            if (this->processor->faceMask[y * processor->W + x] == 255) {
                //check if I have to dilate
                if (AnyNotPixelInMask(x, y, kernelSize)) {
                    //dilate the copy
                    this->processor->faceMaskBackup[y * processor->W + x] = 0;
                }
            }
        }
    }
    memcpy(this->processor->faceMask, this->processor->faceMaskBackup, this->processor->S * this->processor->H / 3);
    ApplyMask();
}

bool FaceDetector::AnyPixelInMask(int x, int y, int kernelSize) {
    int xTemp = x - kernelSize / 2;
    int maxW = this->processor->W - 1;
    int maxH = this->processor->H - 1;
    for (int i = 0; i < kernelSize; i++, xTemp++) {
        //calculate top up y
        int yTemp = y - kernelSize / 2;
        for (int j = 0; j < kernelSize; j++,yTemp++) {
            //if I'm on my pixel go away
            if (xTemp == x && yTemp == y) {
                continue;
            }
            //CROP IMAGE
            if (!(xTemp > 0 && xTemp <= maxW && yTemp > 0 && yTemp <= maxH)) {
                continue;
            }
            //facemask has a 1
            if (this->processor->faceMask[yTemp * processor->W + xTemp] == 255) {
                //check SE has a 1 also
                if (this->SE[j  * kernelSize + i] == 255) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool FaceDetector::AnyNotPixelInMask(int x, int y, int kernelSize) {
    int xTemp = x - kernelSize / 2;
    int maxW = this->processor->W - 1;
    int maxH = this->processor->H - 1;
    for (int i = 0; i < kernelSize; i++, xTemp++) {
        //calculate top up y
        int yTemp = y - kernelSize / 2;
        for (int j = 0; j < kernelSize; j++, yTemp++) {
            //if I'm on my pixel go away
            if (xTemp == x && yTemp == y) {
                continue;
            }
            //CROP IMAGE
            if (!(xTemp > 0 && xTemp <= maxW && yTemp > 0 && yTemp <= maxH)) {
                continue;
            }
            //facemask has a 0
            if (this->processor->faceMask[yTemp * processor->W + xTemp] == 0) {
                //check SE has a 1
                if (this->SE[j  * kernelSize + i] == 255) {
                    return true;
                }
            }
        }
    }
    return false;
}

void FaceDetector::GenerateMask(int kernelSize) {
    if (lastKernelSize == kernelSize) {
        return;
    }
    lastKernelSize = kernelSize;
    if (SE != nullptr) {
        delete[] SE;
    }
    SE = new uchar[kernelSize * kernelSize];
    int r = kernelSize / 2;
    memset(SE, 0, kernelSize * kernelSize);
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dy*dy + dx*dx <= r*r) {
                SE[(r + dy) * kernelSize + (r + dx)] = 255;
            }
        }
    }
}

void FaceDetector::CalculateAverage(int r, int g, int b) {
    sumR += (r * r);
    sumG += (g * g);
    sumB += (b * b);
    int x = average.x;
    int y = average.y;
    int z = average.z;
    if (count > 1) {
        x = average.x * (count - 1);
        y = average.y * (count - 1);
        z = average.z * (count - 1);
    }
    x += r;
    y += g;
    z += b;
    x /= count;
    y /= count;
    z /= count;
    average.x = ImageProcessor::Clamp0255(x);
    average.y = ImageProcessor::Clamp0255(y);
    average.z = ImageProcessor::Clamp0255(z);
}
void FaceDetector::CalculateTypicalDesviation(int r, int g, int b) {
    int x = sumR;
    int y = sumG;
    int z = sumB;
    x /= count;
    y /= count;
    z /= count;
    x -= (average.x * average.x);
    y -= (average.y * average.y);
    z -= (average.z * average.z);
    typicalDesviation.x = ImageProcessor::Clamp0255(sqrt(x));
    typicalDesviation.y = ImageProcessor::Clamp0255(sqrt(y));
    typicalDesviation.z = ImageProcessor::Clamp0255(sqrt(z));
}

FaceDetector::FaceDetector(ImageProcessor* processor) {
    this->sumR = 0;
    this->sumG = 0;
    this->sumB = 0;
    this->count = 0;
    average.x = 0;
    average.y = 0;
    average.z = 0;
    typicalDesviation.x = 0;
    typicalDesviation.y = 0;
    typicalDesviation.z = 0;
    this->processor = processor;
}

FaceDetector::FaceDetector(ImageProcessor* processor, Vector3* average, Vector3* typicalDesviation, int sumR, int sumG, int sumB,int count) {
    this->processor = processor;
    this->average = *average;
    this->typicalDesviation = *typicalDesviation;
    this->sumR = sumR;
    this->sumG = sumG;
    this->sumB = sumB;
    this->count = count;
}

FaceDetector::FaceDetector() {
}


FaceDetector::~FaceDetector() {
}
