#ifndef __AA_BOUNDING_BOX_H__
#define __AA_BOUNDING_BOX_H__
#include "../geom/geometricObject.h"

class AABoundingBox : public GeometricObject
{
	public:
		AABoundingBox(GeometricObject *object);
        virtual void getBounds(float min[3], float max[3]) const;
		virtual ShadeRec hit(Ray &ray) const;
		bool testBBox(Ray &ray) const;
	private:
		GeometricObject *m_object;
		float m_min[3];
		float m_max[3];
};

#endif
