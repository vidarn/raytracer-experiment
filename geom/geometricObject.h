#ifndef __GEOMETRIC_OBJECT_H__
#define __GEOMETRIC_OBJECT_H__
#include "../utils/ray.h"
#include "../utils/rgba.h"

class GeometricObject
{
	public:
		virtual bool hit(const Ray &ray) const = 0;
	protected:
		RGBA m_color;
};

#endif
