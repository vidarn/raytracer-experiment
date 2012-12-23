#ifndef __VIEW_PLANE_H__
#define __VIEW_PLANE_H__
#include "../utils/rgba.h"
#include "../utils/ray.h"
#include "../utils/matrix4x4.h"
#include "../sampler/sampling.h"
#include <vector>
#include <tiffio.h>
#include <tiffio.hxx>

class ViewPlane
{
    public:
        ViewPlane(int resX, int resY, float sizeX, float sizeY, const char *filename);
        ~ViewPlane(){};
        int getNumPixels(){return m_resolution[0]*m_resolution[1];};
        int getWidth(){return m_resolution[0];};
        int getHeight(){return m_resolution[1];};
        Ray getPixelRay(int x, int y, Sampling &sampling);
        void setPixelValue(int x, int y, RGBA color);
        void saveToTiff();
		void setFov(float fov);
		void setFocusDistance(float focusDist){m_focusDist = focusDist;};

        int m_resolution[2];

		friend std::ostream& operator<<(std::ostream &out, ViewPlane &vp);
    private:
		void getDofRay(Ray &ray, Sampling &sampling);
        pthread_mutex_t *m_mutexes;

        std::vector<RGBA> m_pixels;
        std::vector<int> m_numSamples;
        float m_size[2];
		float m_focusDist;
		Vec3 m_origin;
		const char *m_filename;
};

#endif
