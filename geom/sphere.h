#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "geometricObject.h"

class Sphere: public GeometricObject
{
	public:
		Sphere(){};
		Sphere(double radius):m_radius(radius),m_radiusSquared(radius*radius){};
		virtual bool hit(Ray &ray) const;
	private:
		double m_radius;
		double m_radiusSquared;
};

#endif
