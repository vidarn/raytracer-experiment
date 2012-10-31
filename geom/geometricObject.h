#ifndef __GEOMETRIC_OBJECT_H__
#define __GEOMETRIC_OBJECT_H__
#include "../utils/ray.h"
#include "../utils/matrix4x4.h"
#include "../material/material.h"
#include "../shadeRec/shadeRec.h"

class GeometricObject
{
	public:
		virtual ShadeRec hit(Ray &ray) const = 0;
		Material getMaterial(){return m_material;};
		Matrix4x4 getTransform(){return m_transform;};
	protected:
		Material m_material;
		Matrix4x4 m_transform;
};

#endif
