#pragma once

#include <QMainWindow>

class ImageProcessor {

public:
    ImageProcessor(uchar* pixR, uchar* pixG, uchar* pixB, int W, int H, int Padding, int S);
    ~ImageProcessor();

    void UpsideDown();

    void Invert();

    void GrayScale();

    void SepiaFilter();

    void SepiaFilter(uchar thresholdR, uchar thresholdG, uchar thresholdB,uchar diffRG);

    void RadialBlur(int centerX, int centerY);

    void ChangeBrightness(int value);

    void ChangeContrast(int contrast,int brightness);

    static int ContrastTransform(float alpha, float beta, int value);

    static int Clamp0255(int value);

private:
    uchar *pixR, *pixG, *pixB;

    uchar *copyImage;
    uchar *pixRCopy, *pixGCopy, *pixBCopy;

    int W, H, Padding, S;


};

