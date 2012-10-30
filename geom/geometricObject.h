#ifndef __GEOMETRIC_OBJECT_H__
#define __GEOMETRIC_OBJECT_H__
#include "../utils/ray.h"
#include "../utils/rgba.h"
#include "../utils/matrix4x4.h"
#include "../shadeRec/shadeRec.h"

class GeometricObject
{
	public:
		virtual ShadeRec hit(Ray &ray) const = 0;
		RGBA getColor(){return m_color;};
		Matrix4x4 getTransform(){return m_transform;};
	protected:
		RGBA m_color;
		Matrix4x4 m_transform;
};

#endif
