#pragma once

#include <QMainWindow>

class ImageProcessor {

public:
    ImageProcessor(uchar* pixR, uchar* pixG, uchar* pixB, int W, int H, int Padding, int S);
    ~ImageProcessor();

    void UpsideDown();

    void Invert();

    void GrayScale();

    void OldFilter();

private:
    uchar *pixR, *pixG, *pixB;
    int W, H, Padding, S;

    int rOld = 10;
    int gOld = 5;
    int bOld;
};

