#include "point.h"

double PointLight::computeStrength(Point pos)
{
	return 1.0;
}

Vec3 PointLight::computeDirection(Point pos)
{
	Vec3 dir = pos.toVec3();
    dir.invert();
	return dir;
}
