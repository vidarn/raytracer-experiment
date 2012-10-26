#ifndef __PLANE_H__
#define __PLANE_H__
#include "geometricObject.h"
#include "../utils/point.h"
#include "../utils/normal.h"

class Plane: public GeometricObject
{
	public:
		Plane(){};
		Plane(const Point &point, const Normal &normal):m_point(point),m_normal(normal){};
		virtual bool hit(const Ray &ray) const;
	private:
		Point m_point;
		Normal m_normal;
};

#endif
