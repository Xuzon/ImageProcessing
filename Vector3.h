#pragma once

#include <QMainWindow>
#include <cmath>

class Vector3 {
public:
    Vector3();

    Vector3(uchar x, uchar y, uchar z);

    ~Vector3();

    uchar x, y, z;
    
    static float Distance(Vector3* from, Vector3* to);

    static float SqrDistance(Vector3* from, Vector3* to);

    static float AbsDifference(Vector3* from, Vector3* to);

    static float MaxDifference(Vector3* from, Vector3* to);
};

