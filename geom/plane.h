#ifndef __PLANE_H__
#define __PLANE_H__
#include "geometricObject.h"
#include "../utils/point.h"
#include "../utils/normal.h"

class Plane: public GeometricObject
{
	public:
		Plane(Material *material, Matrix4x4 transform){m_transform = transform; m_material = material;};
        virtual void getBounds(float min[3], float max[3]) const;
		virtual ShadeRec hit(Ray &ray) const;
	private:
		Point m_point;
		Normal m_normal;
};

#endif
