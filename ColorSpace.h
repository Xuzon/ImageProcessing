#pragma once
#include <QMainWindow>
#include <math.h>
#define PI 3.1415926535897
class ColorSpace {

public:
    static void RGBtoLHS(uchar* r, uchar* g, uchar* b);
    static void Normalize(float* r, float* g, float* b);
    static void RGBtoXYZ(float* r, float* g, float* b);
    static void XYZtoLAB(float* x, float* y, float* z);
    static void LABtoLHS(float* l, float* a, float* b);
    static inline float ComponentFunction(float f);
    ColorSpace();
    ~ColorSpace();
};

