#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__
#include "light.h"

class PointLight: public Light
{
	public:
		PointLight(float strength, Matrix4x4 transform):Light(strength,transform){};
        virtual float computeStrength(Vec3 pos);
        virtual Vec3 computeDirection(Vec3 pos);
	private:
};

#endif
