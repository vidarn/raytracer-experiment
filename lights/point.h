#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__
#include "light.h"

class PointLight: public Light
{
	public:
		PointLight(std::ifstream &stream, Matrix4x4 transform);
		PointLight(float strength, Matrix4x4 transform);
        virtual float computeStrength(Vec3 &pos);
        virtual Vec3 computeDirection(ShadeRec &sr, Sampler &sampler);
        virtual Ray getRay(Sampler &sampler);
	private:
        Vec3 m_point;
};

#endif
