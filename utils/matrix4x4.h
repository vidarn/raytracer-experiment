#ifndef __MATRIX4X4_H__

#define __MATRIX4X4_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include "vec3.h"
#include <OpenEXR/ImathMatrix.h>

typedef Imath::Matrix44<float> Matrix4x4;

Matrix4x4 matrixFromVector(const Vec3 &vector);
Matrix4x4 matrixFromStream(std::ifstream &stream);

#endif
