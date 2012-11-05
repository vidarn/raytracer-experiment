#ifndef __AA_BOUNDING_BOX_H__
#define __AA_BOUNDING_BOX_H__
#include "../geom/geometricObject.h"

class AABoundingBox : public GeometricObject
{
	public:
		AABoundingBox(GeometricObject *object);
        virtual void getBounds(double min[3], double max[3]) const;
		virtual ShadeRec hit(Ray &ray) const;
		bool testBBox(Ray &ray) const;
	private:
		GeometricObject *m_object;
		double m_min[3];
		double m_max[3];
};

#endif
