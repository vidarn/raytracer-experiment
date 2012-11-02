#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__
#include "light.h"

class PointLight: public Light
{
	public:
		PointLight(double strength, Matrix4x4 transform):Light(strength,transform){};
        virtual double computeStrength(Point pos);
        virtual Vec3 computeDirection(Point pos);
	private:
};

#endif
