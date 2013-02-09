#ifndef __RGBA_H__
#define __RGBA_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include <OpenEXR/ImathColor.h>

typedef Imath::Color4<float> RGBA;

void clamp(RGBA &col);

#endif
