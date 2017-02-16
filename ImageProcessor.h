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


    void ChangeBrightness(int value);

    void ChangeContrast(int contrast,int brightness);

    void Edges(float threshold,EdgeMetric metric);

    Vector3* GetNearbyPixels(int i,int* count,int x);

    void ContrastTransform(float alpha, float beta);

    static int Clamp0255(int value);



private:
    uchar *pixR, *pixG, *pixB;

    uchar LUT[256];

    uchar *copyImage;
    uchar *pixRCopy, *pixGCopy, *pixBCopy;

    int W, H, Padding, S;


};

