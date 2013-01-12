#ifndef __AREALIGHT_H__
#define __AREATLIGHT_H__
#include "light.h"

class AreaLight: public Light
{
	public:
		AreaLight(std::ifstream &stream, Matrix4x4 transform);
        virtual float computeStrength(Vec3 &pos);
        virtual Vec3 computeDirection(ShadeRec &sr, Sampling &sampling);
        virtual Ray getRay(Sampling &sampling);
	private:
        Matrix4x4 m_transform;
        float m_size[2];
};

#endif
