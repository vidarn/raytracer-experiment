#include "material.h"

void Material::setColor(RGBA color)
{
	m_color = color;
}

RGBA Material::shade(ShadeRec shadeRec)
{
	Lambert lambert;
	Vec3 lightDirection = Vec3(1.0,1.0,-1.0);
	lightDirection.normalize();
	double shade = lambert.shade(shadeRec, lightDirection);
	return m_color*shade;
}
