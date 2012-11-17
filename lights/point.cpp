#include "point.h"

float PointLight::computeStrength(Vec3 pos)
{
	return 1.0;
}

Vec3 PointLight::computeDirection(Vec3 pos)
{
	Vec3 dir = pos;
    dir.invert();
	return dir;
}
