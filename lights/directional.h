#ifndef __DIRECTIONAL_H__
#define __DIRECTIONAL_H__
#include "light.h"

class Directional: public Light
{
	public:
		Directional(float strength, Matrix4x4 transform):Light(strength,transform){};
        virtual float computeStrength(Point pos);
        virtual Vec3 computeDirection(Point pos);
	private:
};

#endif
