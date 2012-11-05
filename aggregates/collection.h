#ifndef __COLLECTION_H__
#define __COLLECTION_H__
#include "../geom/geometricObject.h"
#include <vector>

class Collection : public GeometricObject
{
	public:
		Collection();
		void addObject(GeometricObject *object);
        virtual void getBounds(double min[3], double max[3]) const;
		virtual ShadeRec hit(Ray &ray) const;
	private:
		std::vector<GeometricObject*> m_objects;
		double m_min[3];
		double m_max[3];
};

#endif
