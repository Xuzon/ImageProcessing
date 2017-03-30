#include "ColorSpace.h"

void ColorSpace::RGBtoLHS(uchar* r, uchar* g, uchar* b) {
    float l = (*r);
    float h = (*g);
    float s = (*b);
    ColorSpace::RGBtoXYZ(&l, &h, &s);
    ColorSpace::XYZtoLAB(&l, &h, &s);
    ColorSpace::LABtoLHS(&l, &h, &s);
    //normalize degrees
    h = h / 360.0f;
    h = h * 255;
    //normalize saturation
    s *= 2;
    (*r) = (uchar)l;
    (*g) = (uchar)h;
    (*b) = (uchar)s;
}

void ColorSpace::Normalize(float* r, float* g, float* b) {
    (*r) = (*r) > 1 ? (*r) / 255.0f : (*r);
    (*g) = (*g) > 1 ? (*g) / 255.0f : (*g);
    (*b) = (*b) > 1 ? (*b) / 255.0f : (*b);
}

void ColorSpace::RGBtoXYZ(float* r, float* g, float* b) {
    ColorSpace::Normalize(r, g, b);
    float x = 0.412453f * (*r) + 0.357580f * (*g) + 0.180423f * (*b);
    float y = 0.212671f * (*r) + 0.715160f * (*g) + 0.072169f * (*b);
    float z = 0.019334f * (*r) + 0.119193f * (*g) + 0.950227f        * (*b);
    (*r) = x;
    (*g) = y;
    (*b) = z;
}
void ColorSpace::XYZtoLAB(float* x, float* y, float* z) {
    float l = *y > 0.008856 ? 116.0f * pow((*y),1.0f/3.0f) - 16 : 903.3f * (*y);
    float xw = 0.412453f + 0.357580f + 0.180423f;
    float a = 500.0f * (ComponentFunction((*x) / xw) - ComponentFunction(*y));
    float zw = 0.019334f + 0.119193f + 0.950227f;
    float b = 200.0f * (ComponentFunction(*y) - ComponentFunction((*z) / zw));
    (*x) = l * 2.55f;
    (*y) = a;
    (*z) = b;
}

float ColorSpace::ComponentFunction(float f) {
    return f > 0.008856f ? pow(f, 1.0f / 3.0f) : 7.787f * f + 16.0f / 116.0f;;
}

void ColorSpace::LABtoLHS(float* l, float* a, float* b) {
    float distance = sqrt((*a) * (*a) + (*b) + (*b));
    //atan bewteeen b and a
    float angle = atan2((*b) , (*a));
    //convert from -pi to pi TO 0 to 2pi and then convert to degree
    angle = (angle + PI) * 180.0 / PI;
    (*a) = angle;
    (*b) = distance;
}

ColorSpace::ColorSpace() {
}


ColorSpace::~ColorSpace() {
}
