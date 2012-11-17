#ifndef __DIRECTIONAL_H__
#define __DIRECTIONAL_H__
#include "light.h"

class Directional: public Light
{
	public:
		Directional(float strength, Matrix4x4 transform):Light(strength,transform){};
        virtual float computeStrength(Vec3 pos);
        virtual Vec3 computeDirection(Vec3 pos);
	private:
};

#endif
