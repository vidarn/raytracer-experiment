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
        ViewPlane(int resX, int resY, float sizeX, float sizeY, const char *filename);
        ~ViewPlane(){};
        int getNumPixels(){return m_pixels.size();};
        int getWidth(){return m_resolution[0];};
        int getHeight(){return m_resolution[1];};
        Ray getPixelRay(int index, Vec3 sample);
        void setPixelValue(int index, RGBA color);
        void saveToTiff();
		void setFov(float fov);

		friend std::ostream& operator<<(std::ostream &out, ViewPlane &vp);
    private:
        int m_resolution[2];
        std::vector<RGBA> m_pixels;
        float m_size[2];
		Vec3 m_origin;
		const char *m_filename;
};

#endif
