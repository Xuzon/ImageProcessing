#pragma once

#include <QMainWindow>
#include "ColorSpace.h"
#include "Vector3.h"
#include <iostream>

using namespace std;

class FaceDetector;
class ImageProcessor {

public:

    enum EdgeMetric { EuclideanDistance, MaxDifference, AbsDifference };

    ImageProcessor(uchar* pixR, uchar* pixG, uchar* pixB, int W, int H, int Padding, int S);
    ~ImageProcessor();

    



    #pragma region ConvolutionFilters
    //*********CONVOLUTION FILTERS****************
    void Blur(int kernelSize);

    void BlurInsideObjects(int kernelSize, float threshold, EdgeMetric metric);

    void Sharpening(int kernelSize);

    void Edges(float threshold, EdgeMetric metric);
    //********************************************
    #pragma endregion

    #pragma region BasicFilters
    void UpsideDown();

    void Invert();

    void GrayScale();

    void SepiaFilter();

    void SepiaFilter(uchar thresholdR, uchar thresholdG, uchar thresholdB, uchar diffRG);
    #pragma endregion

    #pragma region Dithering
    void Dithering(bool random);

    void AddDitheringNeighborValue(int x, int y, int pos, int neighbor, int luminance);
    #pragma endregion

    #pragma region Brightness&Contrast
    void ChangeBrightness(int value);

    void ChangeContrast(int contrast, int brightness);

    void ContrastTransform(float alpha, float beta);

    void UserContrastTransform();

    void AddToSlope(int x, int y);
    #pragma endregion

    #pragma region Histograms
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
    void NearbyHistograms(int pos, int x, int y, int neighborhood, int* rHistogram, int* gHistogram, int* bHistogram);
    ///
    ///Equalizate histogram only adquiring the value we want
    ///
    void HistogramEqualizationValues(int* histogram, float div, int value, uchar* myLUT);
    #pragma endregion

    #pragma region Aux
    void GetNearbyPixels(Vector3* nearbyPixels, int* count, int x, int y, int* pos, int kernelSize);

    void GetNearbyPixelsOfCopy(Vector3* nearbyPixels, int* count, int x, int y, int* pos, int kernelSize);

    void GetNearbyPixelsInsideObjects(Vector3* nearbyPixels, int* count, int x, int y, int* pos, int kernelSize);

    #pragma endregion

    #pragma region StaticAux
    static float Lerp(float x, float y, float t);

    static uchar Lerp(uchar x, uchar y, float t);

    static int Clamp0255(int value);

    static float Slope(int x1, int x2, int y1, int y2);

    static int Luminance(uchar r, uchar g, uchar b);

    static int Cr(uchar r, uchar g, uchar b);

    static int Cb(uchar r, uchar g, uchar b);
    #pragma endregion



    
    FaceDetector* faceDetector;

    uchar LUT[256];

    uchar rLUT[256];
    uchar gLUT[256];
    uchar bLUT[256];

    int W, H, Padding, S;

    uchar *faceMask;
    uchar *faceMaskBackup;

    uchar *pixR, *pixG, *pixB;

    uchar *pixRCopy, *pixGCopy, *pixBCopy;



    void CreateLHS();
    uchar *pixL, *pixH, *pixS;

private:
    uchar *copyImage;

    std::list<int> cutPoints;
    std::list<float> slopes;

    #pragma region Histograms
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
    #pragma endregion
    
};



class FaceDetector {
public:

    void AddPixel(uchar r, uchar g, uchar b);
    void DetectSkin();
    void DetectSkin(float rDesvMultilier, float gDesvMultiplier, float bDesvMultiplier);

    //Basic object modelling
    void Erode(int kernelSize);
    void Dilate(int kernelSize);
    void ApplyMask();
    //mask operations
    bool AnyPixelInMask(int x, int y, int kernelSize);
    bool AnyNotPixelInMask(int x, int y, int kernelSize);
    void GenerateMask(int kernelSize);
    uchar* SE = nullptr;
    int lastKernelSize = -1;

    FaceDetector(ImageProcessor* processor);
    FaceDetector(ImageProcessor* processor, Vector3* average, Vector3* typicalDesviation, int sumR, int sumG, int sumB, int count);
    FaceDetector();
    ~FaceDetector();
    Vector3 average;
    Vector3 typicalDesviation;
    int sumR = 0;
    int sumG = 0;
    int sumB = 0;
    int count = 0;

private:
    ImageProcessor* processor;
    void CalculateAverage(int r, int g, int b);
    void CalculateTypicalDesviation(int r, int g, int b);
};

