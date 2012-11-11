#ifndef __VIEW_PLANE_H__
#define __VIEW_PLANE_H__
#include "../utils/rgba.h"
#include "../utils/ray.h"
#include "../utils/matrix4x4.h"
#include <vector>
#include <tiffio.h>
#include <tiffio.hxx>

class ViewPlane
{
    public:
        ViewPlane(int resX, int resY, float sizeX, float sizeY);
        ViewPlane(int resX, int resY, float size[2]){ ViewPlane(resX,resY, size[0], size[1]);};
        ViewPlane(int res[2], float size[2]){ ViewPlane(res[0],res[1], size[0], size[1]);};
        ViewPlane(int res[2], float sizeX, float sizeY){ ViewPlane(res[0],res[1], sizeX, sizeY);};
        ~ViewPlane(){};
        int getNumPixels(){return m_pixels.size();};
        int getWidth(){return m_resolution[0];};
        int getHeight(){return m_resolution[1];};
        Ray getPixelRay(int index, Point sample);
        void setPixelValue(int index, RGBA color);
        void saveToTiff(const char *filename);

		friend std::ostream& operator<<(std::ostream &out, ViewPlane &vp);
    private:
        int m_resolution[2];
        std::vector<RGBA> m_pixels;
        float m_size[2];
};

#endif
