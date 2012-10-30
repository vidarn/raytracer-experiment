#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "geometricObject.h"
#include "../utils/matrix4x4.h"

class Sphere: public GeometricObject
{
	public:
		Sphere(RGBA color, Matrix4x4 transform){m_transform = transform; m_color = color;};
		Sphere(double radius, RGBA color, Matrix4x4 transform):m_radius(radius),m_radiusSquared(radius*radius){m_transform = transform; m_color = color;};
		virtual ShadeRec hit(Ray &ray) const;
	private:
		double m_radius;
		double m_radiusSquared;
};

#endif
