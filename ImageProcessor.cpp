#include "ImageProcessor.h"


#pragma region Brightness&Contrast

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

void ImageProcessor::ChangeContrast(int contrast, int brightness) {
    float fContrast = (float)contrast / 50.0;
    float fBright = (((float)brightness / 50.0) - 1) * 255;
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
    UserContrastTransform();
}

///
///Will calculate transference function with user clicked points
///
void ImageProcessor::UserContrastTransform() {
    if (cutPoints.size() == 0) {
        return;
    }
    std::list<int>::iterator it = cutPoints.begin();
    std::list<float>::iterator slopeIt = slopes.begin();
    int nextPoint = *it;
    int lastPoint = 0;
    int lastY = 0;
    int nextY = *slopeIt;
    float slope = Slope(lastPoint, nextPoint, lastY, nextY);
    int val = 0;
    for (int i = 0; i < 256; i++) {
        if (i == nextPoint && i < 255) {
            it++;
            slopeIt++;
            if (it != cutPoints.end()) {
                lastPoint = nextPoint;
                lastY = nextY;
                nextPoint = *it;
                nextY = *slopeIt;
                slope = Slope(lastPoint, nextPoint, lastY, nextY);
            } else {
                lastPoint = nextPoint;
                lastY = nextY;
                nextPoint = 256;
                slope = 1;
            }
        }
        val = slope * (i - lastPoint) + lastY;
        LUT[i] = ImageProcessor::Clamp0255(val);
    }
}

///
///Will add X and Y to the slopes lists
///
void ImageProcessor::AddToSlope(int x, int y) {
    std::list<int>::iterator it = cutPoints.begin();
    bool inserted = false;
    bool duplicated = false;
    int selected = 0;
    if (y > 255 || y < 0) {
        return;
    }
    //search for insert position
    for (selected = 0; it != cutPoints.end(); it++, selected++) {
        //if I have an X same than value is duplicated
        if (*it == x) {
            duplicated = true;
            break;
        }
        //If the current value of iterator is greater than x
        //insert before X
        if (*it > x) {
            cutPoints.insert(it, x);
            inserted = true;
            break;
        }

    }
    //If I didn't inserted and isn't duplicated store at the end
    if (!inserted && !duplicated) {
        cutPoints.push_back(x);
    }

    float slope = y;
    std::list<float>::iterator slopeIt = slopes.begin();
    //set the iterator at right position
    for (int i = 0; i < selected; i++) {
        slopeIt++;
    }
    //if it is duplicated remove it
    if (duplicated) {
        slopeIt = slopes.erase(slopeIt);
    }
    slopes.insert(slopeIt, slope);
}
#pragma endregion

#pragma region BasicFilters
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

void ImageProcessor::SepiaFilter(uchar thresholdR, uchar thresholdG, uchar thresholdB, uchar diffRG) {
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
#pragma endregion

#pragma region Dithering

void ImageProcessor::Dithering(bool random) {

    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int luminance = Clamp0255(Luminance(pixRCopy[i], pixGCopy[i], pixBCopy[i]));
            pixG[i] = pixB[i] = pixR[i] = luminance;
        }
    }

    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int luminance = pixR[i];
            int toSet = 0;
            if (luminance < 128) {
                toSet = 0;
            } else {
                toSet = 255;
            }

            int difference = luminance - toSet;

            float r = random ? (0.9f + rand() % 20 / 100.0f) : 1.0f;
            AddDitheringNeighborValue(x, y, i, 1, r * difference * 7 / 16);

            r = random ? (0.9f + rand() % 20 / 100.0f) : 1.0f;
            AddDitheringNeighborValue(x, y, i, W - 1, r * difference * 3 / 16);

            r = random ? (0.9f + rand() % 20 / 100.0f) : 1.0f;
            AddDitheringNeighborValue(x, y, i, W, r* difference * 5 / 16);

            r = random ? (0.9f + rand() % 20 / 100.0f) : 1.0f;
            AddDitheringNeighborValue(x, y, i, W + 1, r* difference / 16);

            pixR[i] = toSet;
            pixG[i] = toSet;
            pixB[i] = toSet;
        }
    }
}


void ImageProcessor::AddDitheringNeighborValue(int x, int y, int pos, int neighbor, int error) {
    //keep sure crop image
    if (x == 0 && neighbor == W - 1) {
        return;
    }

    if (x == W - 1 && ((neighbor == W + 1) || (neighbor == 1))) {
        return;
    }

    if (y == H - 1 && neighbor != 1) {
        return;
    }

    int  iR = Clamp0255(pixR[pos + 3 * neighbor] + error);
    int iG = Clamp0255(pixG[pos + 3 * neighbor] + error);
    int iB = Clamp0255(pixB[pos + 3 * neighbor] + error);

    /*int  iR = pixR[pos + 3 * neighbor] + error;
    int iG = pixG[pos + 3 * neighbor] + error;
    int iB = pixB[pos + 3 * neighbor] + error;*/


    pixR[pos + 3 * neighbor] = iR;
    pixG[pos + 3 * neighbor] = iG;
    pixB[pos + 3 * neighbor] = iB;
}

#pragma endregion

#pragma region StaticAux
int ImageProcessor::Luminance(uchar r, uchar g, uchar b) {
    return r * 0.2126729f + g * 0.7151522f + b * 0.0721750f;
}

int ImageProcessor::Cr(uchar r, uchar g, uchar b) {
    return -0.147 * r - 0.289 * g + 0.436 * b;
}

int ImageProcessor::Cb(uchar r, uchar g, uchar b) {
    return 0.615 * r - 0.515 * g - 0.100 * b;
}


float ImageProcessor::Slope(int x1, int x2, int y1, int y2) {
    float num = y2 - y1;
    float div = x2 - x1;
    return num / div;
}

int ImageProcessor::Clamp0255(int value) {
    int toRet = value;
    toRet = toRet > 255 ? 255 : toRet;
    toRet = toRet < 0 ? 0 : toRet;
    return toRet;
}

float ImageProcessor::Lerp(float x, float y, float t) {
    return x + t * (y - x);
}

uchar ImageProcessor::Lerp(uchar x, uchar y, float t) {
    return x + t * (y - x);
}
#pragma endregion

#pragma region ConvolutionFilters
///
///Applies a blur to the image
///6 times needed for skin detection
void ImageProcessor::Blur(int kernelSize) {
    int sqKernelMinusOne = kernelSize * kernelSize - 1;
    Vector3* nearbyPixels = new Vector3[sqKernelMinusOne];
    int* buffPos = new int[sqKernelMinusOne];
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            float r = pixR[i];
            float g = pixG[i];
            float b = pixB[i];
            int count = 0;
            GetNearbyPixels(nearbyPixels, &count, x, y, buffPos, kernelSize);
            for (int j = 0; j < count; j++) {
                r += nearbyPixels[j].x;
                g += nearbyPixels[j].y;
                b += nearbyPixels[j].z;
            }

            pixR[i] = r / (count + 1);
            pixG[i] = g / (count + 1);
            pixB[i] = b / (count + 1);
        }
    }
    delete[] nearbyPixels;
    delete[] buffPos;
}

///
///Applies a blur to the objects of image
///
void ImageProcessor::BlurInsideObjects(int kernelSize, float threshold, EdgeMetric metric) {
    this->Edges(threshold, metric);
    int sqKernelMinusOne = kernelSize * kernelSize - 1;
    Vector3* nearbyPixels = new Vector3[sqKernelMinusOne];
    int* buffPos = new int[sqKernelMinusOne];
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            float r = pixRCopy[i];
            float g = pixGCopy[i];
            float b = pixBCopy[i];
            int count = 0;
            GetNearbyPixelsInsideObjects(nearbyPixels, &count, x, y, buffPos, kernelSize);
            for (int j = 0; j < count; j++) {
                r += nearbyPixels[j].x;
                g += nearbyPixels[j].y;
                b += nearbyPixels[j].z;
            }

            pixR[i] = r / (count + 1);
            pixG[i] = g / (count + 1);
            pixB[i] = b / (count + 1);
        }
    }
    delete[] nearbyPixels;
    delete[] buffPos;
}


///
///SHARPENING FILTER
///
void ImageProcessor::Sharpening(int kernelSize) {
    int sqKernelMinusOne = kernelSize * kernelSize - 1;
    Vector3* nearbyPixels = new Vector3[sqKernelMinusOne];
    int* buffPos = new int[sqKernelMinusOne];
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            float r = (sqKernelMinusOne + 1) * pixR[i];
            float g = (sqKernelMinusOne + 1) * pixG[i];
            float b = (sqKernelMinusOne + 1) * pixB[i];
            int count = 0;
            GetNearbyPixelsOfCopy(nearbyPixels, &count, x, y, buffPos, kernelSize);
            for (int j = 0; j < count; j++) {
                r -= nearbyPixels[j].x;
                g -= nearbyPixels[j].y;
                b -= nearbyPixels[j].z;
            }

            pixR[i] = Clamp0255(r);
            pixG[i] = Clamp0255(g);
            pixB[i] = Clamp0255(b);
        }
    }
    delete[] nearbyPixels;
    delete[] buffPos;
}

///
///Show in white (255,255,255) the edges
///
void ImageProcessor::Edges(float threshold, EdgeMetric metric) {
    float calculatedThreshold = threshold;
    if (metric == EdgeMetric::EuclideanDistance) {
        calculatedThreshold *= calculatedThreshold;
    }
    Vector3* pixel = new Vector3(0, 0, 0);
    Vector3 nearbyPixels[9];
    int buffPos[8];
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int r = pixRCopy[i];
            int g = pixGCopy[i];
            int b = pixBCopy[i];
            pixel->SetValues(r, g, b);
            r = g = b = 0;
            int count = 0;
            GetNearbyPixelsOfCopy(nearbyPixels, &count, x, y, buffPos, 3);
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

            pixR[i] = r;
            pixG[i] = g;
            pixB[i] = b;
        }
    }
    delete pixel;
}


#pragma endregion


#pragma region Aux
void ImageProcessor::GetNearbyPixels(Vector3* nearbyPixels, int* count, int x, int y, int* pos, int kernelSize) {
    int nPoints = (kernelSize * kernelSize) - 1;
    for (int i = 0; i < nPoints; i++) {
        pos[i] = -1;
    }
    *count = 0;
    int w = 0;

    //calculate top left X
    int xTemp = x - kernelSize / 2;
    for (int i = 0; i < kernelSize; i++) {
        //calculate top up y
        int yTemp = y - kernelSize / 2;
        for (int j = 0; j < kernelSize; j++) {
            if (xTemp == x && yTemp == y) {
                yTemp++;
                continue;
            }
            //calculate position   
            int tempPos = 0;
            //CROP IMAGE
            if (xTemp < 0 || xTemp >= (W - 1) || yTemp < 0 || yTemp >= (H - 1)) {
                tempPos = -1;
            } else {
                tempPos = yTemp * S + xTemp * 3;
            }
            pos[w] = tempPos;
            w++;
            yTemp++;
        }
        xTemp++;
    }
    //COUNT NOT NULL PIXELS
    for (int j = 0; j < nPoints; j++) {
        if (pos[j] > -1 && pos[j] < S * H) {
            (*count)++;
        } else {
            pos[j] = -1;
        }
    }
    //allocate array
    int k = 0;
    //asign
    for (int j = 0; j < nPoints; j++) {
        int currPos = pos[j];
        if (currPos > -1) {
            nearbyPixels[k].x = pixR[currPos];
            nearbyPixels[k].y = pixG[currPos];
            nearbyPixels[k].z = pixB[currPos];
            k++;
        }
    }
}

void ImageProcessor::GetNearbyPixelsOfCopy(Vector3* nearbyPixels, int* count, int x, int y, int* pos, int kernelSize) {
    int nPoints = (kernelSize * kernelSize) - 1;
    for (int i = 0; i < nPoints; i++) {
        pos[i] = -1;
    }
    *count = 0;
    int w = 0;

    //calculate top left X
    int xTemp = x - kernelSize / 2;
    for (int i = 0; i < kernelSize; i++) {
        //calculate top up y
        int yTemp = y - kernelSize / 2;
        for (int j = 0; j < kernelSize; j++) {
            if (xTemp == x && yTemp == y) {
                yTemp++;
                continue;
            }
            //calculate position   
            int tempPos = 0;
            //CROP IMAGE
            if (xTemp < 0 || xTemp >= (W - 1) || yTemp < 0 || yTemp >= (H - 1)) {
                tempPos = -1;
            } else {
                tempPos = yTemp * S + xTemp * 3;
            }
            pos[w] = tempPos;
            w++;
            yTemp++;
        }
        xTemp++;
    }
    //COUNT NOT NULL PIXELS
    for (int j = 0; j < nPoints; j++) {
        if (pos[j] > -1 && pos[j] < S * H) {
            (*count)++;
        } else {
            pos[j] = -1;
        }
    }
    //allocate array
    int k = 0;
    //asign
    for (int j = 0; j < nPoints; j++) {
        int currPos = pos[j];
        if (currPos > -1) {
            nearbyPixels[k].x = pixRCopy[currPos];
            nearbyPixels[k].y = pixGCopy[currPos];
            nearbyPixels[k].z = pixBCopy[currPos];
            k++;
        }
    }
}

void ImageProcessor::GetNearbyPixelsInsideObjects(Vector3* nearbyPixels, int* count, int x, int y, int* pos, int kernelSize) {
    //-1 = invalid pixel
    //-2 = edge
    int nPoints = (kernelSize * kernelSize) - 1;
    for (int i = 0; i < nPoints; i++) {
        pos[i] = -1;
    }
    *count = 0;
    int w = 0;

    //calculate top left X
    int xTemp = x - kernelSize / 2;
    for (int i = 0; i < kernelSize; i++) {
        //calculate top up y
        int yTemp = y - kernelSize / 2;
        for (int j = 0; j < kernelSize; j++) {
            if (xTemp == x && yTemp == y) {
                yTemp++;
                continue;
            }
            //calculate position   
            int tempPos = 0;
            //CROP IMAGE
            if (xTemp < 0 || xTemp >= (W - 1) || yTemp < 0 || yTemp >= (H - 1)) {
                tempPos = -1;
            } else {
                tempPos = yTemp * S + xTemp * 3;
            }
            if (tempPos == -1 || tempPos == -2) {
                pos[w] = tempPos;
                w++;
                yTemp++;
                continue;
            }
            //if it is an edge
            if (pixR[tempPos] == 255 && pixG[tempPos] == 255 && pixB[tempPos] == 255) {
                tempPos = -2;
            }

            //Doesn't count if it is at the other side of an edge
            for (int q = 0; q < kernelSize; q++) {
                int lerpedX = Lerp((float)xTemp, x, q / (float)kernelSize);
                int lerpedY = Lerp((float)yTemp, y, q / (float)kernelSize);
                int lerpedPos = lerpedY * S + lerpedX * 3;
                //check if is an edge between the position and the center pixel
                if (pixR[lerpedPos] == 255 && pixG[lerpedPos] == 255 && pixB[lerpedPos] == 255) {
                    tempPos = -1;
                }
            }
            pos[w] = tempPos;
            w++;
            yTemp++;
        }
        xTemp++;
    }
    //COUNT NOT NULL PIXELS
    for (int j = 0; j < nPoints; j++) {
        if (pos[j] > -1 && pos[j] < S * H) {
            (*count)++;
        } else {
            pos[j] = -1;
        }
    }
    //allocate array
    int k = 0;
    //asign
    for (int j = 0; j < nPoints; j++) {
        int currPos = pos[j];
        if (currPos > -1) {
            nearbyPixels[k].x = pixRCopy[currPos];
            nearbyPixels[k].y = pixGCopy[currPos];
            nearbyPixels[k].z = pixBCopy[currPos];
            k++;
        }
    }
}

#pragma endregion

#pragma region Histograms
///
///Returns the histograms normalized to the maximum value of each
///histogram to 99
int ImageProcessor::Histograms(int* rHistogram, int* gHistogram, int* bHistogram, int* rRawHistogram, int* gRawHistogram, int* bRawHistogram) {

    int toRet = 0;

    memset(rHistogram, 0, 256 * sizeof(int));
    memset(gHistogram, 0, 256 * sizeof(int));
    memset(bHistogram, 0, 256 * sizeof(int));

    memset(rRawHistogram, 0, 256 * sizeof(int));
    memset(gRawHistogram, 0, 256 * sizeof(int));
    memset(bRawHistogram, 0, 256 * sizeof(int));

    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            int r = pixR[i];
            int g = pixG[i];
            int b = pixB[i];
            rHistogram[r]++;
            gHistogram[g]++;
            bHistogram[b]++;
            rRawHistogram[r]++;
            gRawHistogram[g]++;
            bRawHistogram[b]++;
        }
    }

    int rMaxValue = 0;
    for (int i = 0; i < 256; i++) {
        if (rHistogram[i] > rMaxValue) {
            rMaxValue = rHistogram[i];
        }
    }

    int gMaxValue = 0;
    for (int i = 0; i < 256; i++) {
        if (gHistogram[i] > gMaxValue) {
            gMaxValue = gHistogram[i];
        }
    }

    int bMaxValue = 0;
    for (int i = 0; i < 256; i++) {
        if (bHistogram[i] > bMaxValue) {
            bMaxValue = bHistogram[i];
        }
    }

    toRet = rMaxValue;
    if (bMaxValue > toRet) {
        toRet = bMaxValue;
    }

    if (gMaxValue > toRet) {
        toRet = gMaxValue;
    }

    for (int i = 0; i < 256; i++) {
        rHistogram[i] = (rHistogram[i] * 99) / rMaxValue;
        gHistogram[i] = (gHistogram[i] * 99) / gMaxValue;
        bHistogram[i] = (bHistogram[i] * 99) / bMaxValue;
    }
    return toRet;
}

///
/// Stret linearly the image (Histogram)
///
void ImageProcessor::LinearStretch(int* rHistogram, int* gHistogram, int* bHistogram) {

    StretchHistogram(rHistogram, pixR);
    StretchHistogram(gHistogram, pixG);
    StretchHistogram(bHistogram, pixB);
}

///
///Get init and end values of histogram and then apply the linear stretch
///
void ImageProcessor::StretchHistogram(int* histogram, uchar* componentPointer) {
    int lowFreq = 0;
    int highFreq = 0;
    int maxValuePos = 0;
    //get ini and end of histogram
    for (int i = 0; i < 256; i++) {
        if (histogram[i] != 0) {
            highFreq = i;
            if (lowFreq == 0) {
                lowFreq = i;
            }
            if (histogram[i] > maxValuePos) {
                maxValuePos = i;
            }
        }
    }
    //apply it to the image
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            componentPointer[i] = LinearStretchInterpolation(lowFreq, highFreq, maxValuePos, histogram,componentPointer[i]);
        }
    }

}

///
///Apply the linear stretch interpolation to a value
///
uchar ImageProcessor::LinearStretchInterpolation(int iniPos, int endPos, int maxValuePos, int* histo, int value) {
    uchar toRet = value;
    //TODO
    float num = value - iniPos;
    float div = endPos - iniPos;
    float x = num / div;
    value = x * 255;
    toRet = Clamp0255(value);
    return toRet;
}

///
///Apply the Vogue stretch interpolation to a value
///
uchar ImageProcessor::VogueStretchInterpolation(int iniPos, int endPos, int maxValuePos, int* histo, int value) {
    uchar toRet = value;
    //TODO
    if (value < maxValuePos) {
        float num = value - iniPos;
        float div = maxValuePos - iniPos;
        float x = num / div;
        value = Clamp0255(x * maxValuePos);
    } else {
        if (value > maxValuePos) {
            float num = value - maxValuePos;
            float div = endPos - maxValuePos;
            float x = num / div;
            value = maxValuePos + x * endPos;
        }
    }
    toRet = Clamp0255(value);
    return toRet;
}


///
///Equalize a histogram
///
void ImageProcessor::HistogramEqualization(int* rHistogram, int* gHistogram, int* bHistogram, float div) {
    //LUT[i] = sum of column and its predecessors divided by num of pixels
    //of image multiplied by 255
    float r = 0;
    float g = 0;
    float b = 0;
    for (int i = 0; i < 256; i++) {
        r += rHistogram[i];
        g += gHistogram[i];
        b += bHistogram[i];

        rLUT[i] = Clamp0255((int)(r * 255.0f / div));
        gLUT[i] = Clamp0255((int)(g * 255.0f / div));
        bLUT[i] = Clamp0255((int)(b * 255.0f / div));
    }
}

///
///Apply the equalization by histogram
///
void ImageProcessor::ApplyHistogramEqualization() {
    //apply it to the image
    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            pixR[i] = rLUT[pixR[i]];
            pixG[i] = gLUT[pixG[i]];
            pixB[i] = bLUT[pixB[i]];
        }
    }
}

///
///Apply the equalization of histogram in neighborhood
///
void ImageProcessor::AdapativeHistogramEqualization(int neighborhood) {
    int rHistogram[256];
    int gHistogram[256];
    int bHistogram[256];

    for (int y = 0, i = 0; y < H; y++, i += Padding) {
        for (int x = 0; x < W; x++, i += 3) {
            //get nearby histograms
            NearbyHistograms(i,x,y,neighborhood, rHistogram, gHistogram, bHistogram);
            //equalize nerby histograms
            this->HistogramEqualizationValues(rHistogram, neighborhood * neighborhood, pixR[i], rLUT);
            this->HistogramEqualizationValues(gHistogram, neighborhood * neighborhood, pixG[i], gLUT);
            this->HistogramEqualizationValues(bHistogram, neighborhood * neighborhood, pixB[i], bLUT);
            //apply them
            pixR[i] = rLUT[pixR[i]];
            pixG[i] = gLUT[pixG[i]];
            pixB[i] = bLUT[pixB[i]];
        }
    }
}

///
///Calculate nearby histograms
///
void ImageProcessor::NearbyHistograms(int pos, int x, int y,int neighborhood, int* rHistogram, int* gHistogram, int* bHistogram) {
    //TODO optimize
    memset(rHistogram, 0, 256 * sizeof(int));
    memset(gHistogram, 0, 256 * sizeof(int));
    memset(bHistogram, 0, 256 * sizeof(int));
    int xTemp = 0;
    int yTemp = 0;
    int tempPos = 0;
    int temp = neighborhood / 2;
    for (int i = 0; i < neighborhood; i++) {
        int col = i - temp;
        xTemp = x + col;
        if (xTemp < 0 || xTemp > W) {
            continue;
        }
        for (int j = 0; j < neighborhood; j++) {
            int row = j - temp;
            yTemp = y + row;
            //check borders
            if (yTemp > H || yTemp < 0) {
                continue;
            }
            //calculate pos
            tempPos = yTemp * S + xTemp * 3;
            //add to the histograms
            rHistogram[pixR[tempPos]]++;
            gHistogram[pixG[tempPos]]++;
            bHistogram[pixB[tempPos]]++;
        }
    }
}

///
///Equalizate histogram only adquiring the value we want
///
void ImageProcessor::HistogramEqualizationValues(int* histogram, float div, int value, uchar* myLUT) {
    //LUT[i] = sum of column and its predecessors divided by num of pixels
    //of image multiplied by 255
    float val = 0;
    for (int i = 0; i <= value; i++) {
        val += histogram[i];
    }
    myLUT[value] = Clamp0255((int)(val * 255.0f / div));
}

#pragma endregion

void ImageProcessor::CreateLHS() {
    if (pixL) {
        delete[] pixL;
    }
    pixL = new uchar[S * H];
    pixS = (pixH = pixL + 1) + 1;
    memcpy(this->pixL, this->pixR, H * S);
    for (int i = 0; i < H * S; i += 3) {
        ColorSpace::RGBtoLHS(&pixL[i], &pixH[i], &pixS[i]);
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
    this->faceMask = new uchar[S * H / 3];
    this->matFaceMask = cv::Mat(H, W, CV_8U, this->faceMask, W);
    this->faceMaskBackup = new uchar[S * H / 3];
    memcpy(this->copyImage, this->pixR,H * S);
    pixBCopy = (pixGCopy = (pixRCopy = copyImage) + 1) + 1;
    for (int i = 0; i < 256; i++) {
        LUT[i] = i;
    }
    //this->faceDetector = new FaceDetector(this);
    this->pixL = NULL;
    this->CreateLHS();
    //this->faceDetector = new FaceDetector(this);
    /*this->faceDetector = new FaceDetector(this,new Vector3(125,138,7),new Vector3(154,71,17), 5884620
        , 3607851, 53067,149);*/
    /*this->faceDetector = new FaceDetector(this,new Vector3(200,152,11),new Vector3(68,49,16), 2009234
        , 1150710, 17087,45);*/
    this->faceDetector = new FaceDetector(this,new Vector3(202,151,16),new Vector3(43,41,11), 982537
    , 564648, 9049,23);
}

ImageProcessor::~ImageProcessor() {
    if (copyImage != nullptr) {
        delete[] copyImage;
        delete[] pixL;
        delete[] faceMask;
        delete[] faceMaskBackup;
    }
    delete this->faceDetector;
}
