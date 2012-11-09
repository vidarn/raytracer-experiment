#include "directional.h"

float Directional::computeStrength(Point pos)
{
	return 1.0;
}

Vec3 Directional::computeDirection(Point pos)
{
	Vec3 dir(0.0,0.0,-1.0);
	return dir;
}
