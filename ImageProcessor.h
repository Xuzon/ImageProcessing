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

    ///
    ///Returns the histograms normalized to the maximum value of each
    ///histogram to 99
    int Histograms(int* rHistogram, int* gHistogram, int* bHistogram, int* rRawHistogram, int* gRawHistogram, int* bRawHistogram);

    ///
    /// Stretch linearly the image (Histogram)
    ///
    void LinearStretch(int* rHistogram, int* gHistogram, int* bHistogram);

    ///
    ///Equalizate histograms
    ///
    void HistogramEqualization(int* rHistogram, int* gHistogram, int* bHistogram, float div);

    ///
    ///Apply the equalization by histogram
    ///
    void ApplyHistogramEqualization();

    ///
    ///Apply the equalization of histogram in neighborhood
    ///
    void AdapativeHistogramEqualization(int neighborhood);

    ///
    ///Calculate nearby histograms
    ///
    void NearbyHistograms(int pos, int x, int y,int neighborhood, int* rHistogram, int* gHistogram, int* bHistogram);
    ///
    ///Equalizate histogram only adquiring the value we want
    ///
    void HistogramEqualizationValues(int* histogram,float div,int value, uchar* myLUT);


    static float Lerp(float x, float y, float t);

    static uchar Lerp(uchar x, uchar y, float t);

    static int Clamp0255(int value);

    static float Slope(int x1, int x2, int y1, int y2);

    static int Luminance(uchar r, uchar g, uchar b);

    static int Cr(uchar r, uchar g, uchar b);

    static int Cb(uchar r, uchar g, uchar b);

    uchar LUT[256];

    uchar rLUT[256];
    uchar gLUT[256];
    uchar bLUT[256];

    void AddToSlope(int x, int y);

private:
    uchar *pixR, *pixG, *pixB;

    uchar *copyImage;
    uchar *pixRCopy, *pixGCopy, *pixBCopy;

    int W, H, Padding, S;

    std::list<int> cutPoints;
    std::list<float> slopes;

    ///
    ///Get init and end values of histogram and then apply the linear stretch
    ///
    void StretchHistogram(int* histogram, uchar* componentPointer);

    ///
    ///Apply the linear stretch interpolation to a value
    ///
    uchar LinearStretchInterpolation(int iniPos, int endPos, int maxValuePos, int* histo, int value);

    ///
    ///Apply the Vogue stretch interpolation to a value
    ///
    uchar VogueStretchInterpolation(int iniPos, int endPos, int maxValuePos, int* histo, int value);
};

