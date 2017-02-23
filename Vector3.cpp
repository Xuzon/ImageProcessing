#include "Vector3.h"



Vector3::Vector3() {
}

Vector3::Vector3(uchar x, uchar y, uchar z) {
    this->x = x;
    this->y = y;
    this->z = z;
}


Vector3::~Vector3() {
}

//NORMA 0
//valor absoluto diferencia vectores
//NORMA 3
//maximo diferencias


void Vector3::SetValues(uchar x, uchar y, uchar z) {
    this->x = x;
    this->y = y;
    this->z = z;
}


float Vector3::Distance(Vector3* from, Vector3* to) {
    float toRet = 0;

    float x = to->x - from->x;
    float y = to->y - from->y;
    float z = to->z - from->z;

    x *= x;
    y *= y;
    z *= z;

    toRet = sqrt(x + y + z);

    return toRet;
}

float Vector3::SqrDistance(Vector3* from, Vector3* to) {
    float toRet = 0;

    float x = to->x - from->x;
    float y = to->y - from->y;
    float z = to->z - from->z;

    x *= x;
    y *= y;
    z *= z;

    toRet = x + y + z;

    return toRet;
}

float Vector3::AbsDifference(Vector3* from, Vector3* to) {
    float x = to->x - from->x;
    float y = to->y - from->y;
    float z = to->z - from->z;
    return abs(x) + abs(y) + abs(z);
}

float Vector3::MaxDifference(Vector3* from, Vector3* to) {
    float x = to->x - from->x;
    float y = to->y - from->y;
    float z = to->z - from->z;
    float max = abs(x);
    int i = abs(y);
    if (i > max) {
        max = i;
    }
    i = abs(z);
    if (i > max) {
        max = i;
    }
    return max;
}
