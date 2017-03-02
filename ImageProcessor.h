#pragma once

#include <QMainWindow>
#include "Vector3.h"

class ImageProcessor {

public:

    enum EdgeMetric { EuclideanDistance, MaxDifference, AbsDifference };

    ImageProcessor(uchar* pixR, uchar* pixG, uchar* pixB, int W, int H, int Padding, int S);
    ~ImageProcessor();

    void UpsideDown();

    void Invert();

    void GrayScale();

    void SepiaFilter();

    void SepiaFilter(uchar thresholdR, uchar thresholdG, uchar thresholdB,uchar diffRG);

    void Dithering(bool random);

    void AddDitheringNeighborValue(int x, int y,int pos,int neighbor,int luminance);

    void ChangeBrightness(int value);

    void ChangeContrast(int contrast,int brightness);

    void Edges(float threshold,EdgeMetric metric);

    void GetNearbyPixels(Vector3* nearbyPixels, int i, int* count, int x, int* pos);

    void ContrastTransform(float alpha, float beta);

    void UserContrastTransform();

    void Histograms(int* rHistogram, int* gHistogram, int* bHistogram);

    static float Lerp(float x, float y, float t);

    static uchar Lerp(uchar x, uchar y, float t);

    static int Clamp0255(int value);

    static int Luminance(uchar r, uchar g, uchar b);

    static int Cr(uchar r, uchar g, uchar b);

    static int Cb(uchar r, uchar g, uchar b);

    uchar LUT[256];

    void AddToSlope(int x, int y);

private:
    uchar *pixR, *pixG, *pixB;

    uchar *copyImage;
    uchar *pixRCopy, *pixGCopy, *pixBCopy;

    int W, H, Padding, S;

    std::list<int> cutPoints;
    std::list<float> slopes;
};

