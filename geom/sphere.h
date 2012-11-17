#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "geometricObject.h"
#include "../utils/matrix4x4.h"

class Sphere: public GeometricObject
{
	public:
		Sphere(Material *material, Matrix4x4 transform){m_transform = transform; m_material = material;};
		Sphere(float radius, Material *material, Matrix4x4 transform):m_radius(radius),m_radiusSquared(radius*radius){m_transform = transform; m_material = material;};
        Sphere(std::istream &stream, Matrix4x4 transform, Material *material);
        virtual void getBounds(float min[3], float max[3]) const;
		virtual void hit(Ray &ray, ShadeRec &sr) const;
	private:
		float m_radius;
		float m_radiusSquared;
};

#endif
