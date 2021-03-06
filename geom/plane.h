#ifndef __PLANE_H__
#define __PLANE_H__
#include "geometricObject.h"

class Plane: public GeometricObject
{
	public:
		Plane(Material *material, Matrix4x4 transform){m_transform = transform; m_material = material;};
        virtual void getBounds(float min[3], float max[3]) const;
		virtual void hit(Ray &ray, ShadeRec &sr) const;
	private:
		Vec3 m_point;
		Vec3 m_normal;
};

#endif
