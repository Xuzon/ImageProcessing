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
            uchar toSet = skin ? 1 : 0;
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
            processor->faceMask[i / 3] = skin ? 1 : 0;
        }
    }

    this->ApplyMask();
}

void FaceDetector::ApplyMask() {
    /*for (int y = 0, i = 0; y < processor->H; y++, i += processor->Padding) {
        for (int x = 0; x < processor->W; x++, i += 3) {
            bool skin = processor->faceMask[i / 3] == 1 ? true : false;
            processor->pixR[i] = skin ? processor->pixRCopy[i] : 0;
            processor->pixG[i] = skin ? processor->pixGCopy[i] : 0;
            processor->pixB[i] = skin ? processor->pixBCopy[i] : 0;
        }
    }*/
    for (int y = 0, i = 0; y < processor->H; y++, i += processor->Padding) {
        for (int x = 0; x < processor->W; x++, i += 3) {
            processor->pixR[i] = processor->pixRCopy[i];
            processor->pixG[i] = processor->pixGCopy[i];
            processor->pixB[i] = processor->pixBCopy[i];
        }
    }
    LabelFaceMask();
}

void FaceDetector::Dilate(int kernelSizeX,int kernelSizeY) {
    GenerateMask(kernelSizeX,kernelSizeY);
    for (int y = 0; y < processor->H; y++) {
        for (int x = 0; x < processor->W; x++) {
            //set the copy value
            this->processor->faceMaskBackup[y * processor->W + x] = this->processor->faceMask[y * processor->W + x];
            //if my facemask has a 0
            if (this->processor->faceMask[y * processor->W + x] == 0){
                //check if I have to dilate
                if (AnyPixelInMask(x, y, kernelSizeX,kernelSizeY)) {
                    //dilate the copy
                    this->processor->faceMaskBackup[y * processor->W + x] = 1;
                }
            } 
        }
    }
    memcpy(this->processor->faceMask, this->processor->faceMaskBackup,this->processor->S * this->processor->H / 3);
    ApplyMask();
}

void FaceDetector::Erode(int kernelSizeX,int kernelSizeY) {
    GenerateMask(kernelSizeX,kernelSizeY);
    for (int y = 0; y < processor->H; y++) {
        for (int x = 0; x < processor->W; x++) {
            //set the copy value
            this->processor->faceMaskBackup[y * processor->W + x] = this->processor->faceMask[y * processor->W + x];
            //if my facemask has a 0
            if (this->processor->faceMask[y * processor->W + x] == 1) {
                //check if I have to dilate
                if (AnyNotPixelInMask(x, y, kernelSizeX,kernelSizeY)) {
                    //dilate the copy
                    this->processor->faceMaskBackup[y * processor->W + x] = 0;
                }
            }
        }
    }
    memcpy(this->processor->faceMask, this->processor->faceMaskBackup, this->processor->S * this->processor->H / 3);
    ApplyMask();
}

bool FaceDetector::AnyPixelInMask(int x, int y, int kernelSizeX,int kernelSizeY) {
    int xTemp = x - kernelSizeX / 2;
    int maxW = this->processor->W - 1;
    int maxH = this->processor->H - 1;
    for (int i = 0; i < kernelSizeX; i++, xTemp++) {
        //calculate top up y
        int yTemp = y - kernelSizeY / 2;
        for (int j = 0; j < kernelSizeY; j++,yTemp++) {
            //if I'm on my pixel go away
            if (xTemp == x && yTemp == y) {
                continue;
            }
            //CROP IMAGE
            if (!(xTemp > 0 && xTemp <= maxW && yTemp > 0 && yTemp <= maxH)) {
                continue;
            }
            //facemask has a 1
            if (this->processor->faceMask[yTemp * processor->W + xTemp] == 1) {
                //check SE has a 1 also
                if (this->SE[j  * kernelSizeX + i] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool FaceDetector::AnyNotPixelInMask(int x, int y, int kernelSizeX,int kernelSizeY) {
    int xTemp = x - kernelSizeX / 2;
    int maxW = this->processor->W - 1;
    int maxH = this->processor->H - 1;
    for (int i = 0; i < kernelSizeX; i++, xTemp++) {
        //calculate top up y
        int yTemp = y - kernelSizeY / 2;
        for (int j = 0; j < kernelSizeY; j++, yTemp++) {
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
                if (this->SE[j  * kernelSizeX + i] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

///
///GENERATE PATTERN MASK (ELLIPSE PATTERN)
///
void FaceDetector::GenerateMask(int kernelSizeX,int kernelSizeY) {
    //TODO generate mask different X and Y
    if (lastKernelSizeX == kernelSizeX && lastKernelSizeY == kernelSizeY) {
        return;
    }
    lastKernelSizeX = kernelSizeX;
    lastKernelSizeY = kernelSizeY;
    //TODO something breaks at delete SE
    if (SE != nullptr) {
        delete[] SE;
    }
    SE = new uchar[kernelSizeX * kernelSizeY];
    memset(SE, 0, kernelSizeX * kernelSizeY);
    int height = kernelSizeY / 2;
    int width = kernelSizeX / 2;
    int origin = width * height / 2 + width / 2;
    for (int y = -height; y <= height; y++) {
        for (int x = -width; x <= width; x++) {
            if (x*x*height*height + y*y*width*width <= height*height*width*width) {
                if (origin + x + y * width >= kernelSizeX * kernelSizeY) {
                    printf("HOLA");
                }
                SE[origin + x + y * width] = 1;

            }
        }
    }
}

///
///Label blobs
///
void FaceDetector::LabelFaceMask() {
    int counter = 0;
    if (iFaceMask != nullptr) {
        delete[] iFaceMask;
    }
    iFaceMask = new int[this->processor->H * this->processor->W];
    for (int y = 0; y < processor->H; y++) {
        for (int x = 0; x < processor->W; x++) {
            int pos = y * this->processor->W + x;
            iFaceMask[pos] = this->processor->faceMask[pos];
            iFaceMask[pos] = iFaceMask[pos] == 1 ? -1 : 0;
        }
    }
    label = 0;
    for (int y = 0; y < processor->H; y++) {
        for (int x = 0; x < processor->W; x++) {
            if (iFaceMask[y * this->processor->W + x] < 0) {
                safe = 0;
                minX = x;
                maxX = x;
                minY = y;
                maxY = y;
                label++;
                SetLabel(x, y);
                DrawBox(minX, minY, maxX, maxY);
            }
        }
    }
    /*//Debug blobs
    for (int y = 0, i = 0; y < this->processor->H; y++, i += this->processor->Padding) {
        for (int x = 0; x < this->processor->W; x++, i += 3) {
            this->processor->pixR[i] = 13 * iFaceMask[i / 3];
            this->processor->pixG[i] = 37 * iFaceMask[i / 3];
            this->processor->pixB[i] = 51 * iFaceMask[i / 3];
        }
    }*/
}

///
///Set the label into the iFaceMask of blobs and
///set some parameters
void FaceDetector::SetLabel(int x, int y) {
    safe++;
    //don't make a stack overflow
    if (safe > MAX_CALLSTACK) {
        return;
    }
    //choosen parameters
    if (x < minX) {
        minX = x;
    }
    if (x > maxX) {
        maxX = x;
    }
    if (y < minY) {
        minY = y;
    }
    if (y > maxY) {
        maxY = y;
    }
    //set label
    iFaceMask[y * this->processor->W + x] = label;
    //upper pixel
    if ((y - 1 >= 0) && iFaceMask[(y - 1) * this->processor->W + x] < 0) {
        SetLabel(x,(y - 1));
    }
    //under pixel
    if ((y + 1 < this->processor->H) && iFaceMask[(y + 1) * this->processor->W + x] < 0) {
        SetLabel(x, y + 1);
    }
    //left pixel
    if ((x - 1 >= 0) && iFaceMask[y * this->processor->W + x - 1] < 0) {
        SetLabel(x - 1, y);
    }
    //right pixel
    if ((x + 1 < this->processor->W) && iFaceMask[y * this->processor->W + x + 1] < 0) {
        SetLabel(x + 1, y);
    }
    safe--;
}

///
///Draw box into the ImageProcessor image
///
void FaceDetector::DrawBox(int minX, int minY, int maxX, int maxY) {
    for (int i = minX; i < maxX; i++) {
        //draw from down to up
        int pos = (minY * this->processor->W + i) * 3;
        this->processor->pixR[pos] = 255;
        this->processor->pixG[pos] = 0;
        this->processor->pixB[pos] = 255;
        pos = (maxY * this->processor->W + i) * 3;
        this->processor->pixR[pos] = 255;
        this->processor->pixG[pos] = 0;
        this->processor->pixB[pos] = 255;
        if (i == minX || i == (maxX - 1)) {
            for (int j = minY; j < maxY; j++) {
                int tempPos = (j * this->processor->W + i ) * 3;
                this->processor->pixR[tempPos] = 255;
                this->processor->pixG[tempPos] = 0;
                this->processor->pixB[tempPos] = 255;
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
    this->iFaceMask = nullptr;
}

FaceDetector::FaceDetector() {
}


FaceDetector::~FaceDetector() {
}
