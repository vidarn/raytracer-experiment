#ifndef __AREALIGHT_H__
#define __AREATLIGHT_H__
#include "light.h"

class AreaLight: public Light
{
	public:
		AreaLight(std::ifstream &stream, Matrix4x4 transform);
        virtual float computeStrength(Vec3 &dir, Vec3 &point, Vec3 &samplePos, float *pdf); virtual Vec3 computeDirection(ShadeRec &sr, Sampler &sampler);
        virtual Ray getRay(Sampler &sampler);
	private:
        Matrix4x4 m_transform;
        float m_size[2];
        Vec3 m_normal;
        float m_area;
};

#endif
