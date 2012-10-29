#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "geometricObject.h"

class Sphere: public GeometricObject
{
	public:
		Sphere(RGBA color){m_color = color;};
		Sphere(double radius, RGBA color):m_radius(radius),m_radiusSquared(radius*radius){m_color = color;};
		virtual ShadeRec hit(Ray &ray) const;
	private:
		double m_radius;
		double m_radiusSquared;
};

#endif
