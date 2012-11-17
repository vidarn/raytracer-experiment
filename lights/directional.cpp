#include "directional.h"

float Directional::computeStrength(Vec3 pos)
{
	return 1.0;
}

Vec3 Directional::computeDirection(Vec3 pos)
{
	Vec3 dir(0.0,0.0,-1.0);
	return dir;
}
