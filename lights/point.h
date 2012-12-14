#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__
#include "light.h"

class PointLight: public Light
{
	public:
		PointLight(std::ifstream &stream, Matrix4x4 transform);
		PointLight(float strength, Matrix4x4 transform);
        virtual float computeStrength(Vec3 &pos);
        virtual Vec3 computeDirection(Vec3 &pos, Sampling &sampling);
	private:
        Vec3 m_point;
};

#endif
