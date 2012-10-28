#ifndef __VIEW_PLANE_H__
#define __VIEW_PLANE_H__
#include "../utils/rgba.h"
#include <vector>

class ViewPlane
{
    public:
        ViewPlane(int resX, int resY, double sizeX, double sizeY);
        ViewPlane(int resX, int resY, double size[2]){ ViewPlane(resX,resY, size[0], size[1]);};
        ViewPlane(int res[2], double size[2]){ ViewPlane(res[0],res[1], size[0], size[1]);};
        ViewPlane(int res[2], double sizeX, double sizeY){ ViewPlane(res[0],res[1], sizeX, sizeY);};
        ~ViewPlane(){};
    private:
        int m_resolution[2];
        std::vector<RGBA> m_pixels;
        double m_size[2];
};

#endif
