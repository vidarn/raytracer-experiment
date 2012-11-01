#ifndef __DIRECTIONAL_H__
#define __DIRECTIONAL_H__
#include "light.h"

class Directional: public Light
{
	public:
		Directional(double strength, Matrix4x4 transform):Light(strength,transform){};
        virtual double computeStrength(Point pos);
        virtual Vec3 computeDirection(Point pos);
	private:
};

#endif
