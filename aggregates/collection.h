#ifndef __COLLECTION_H__
#define __COLLECTION_H__
#include "../geom/geometricObject.h"
#include <vector>

class Collection : public GeometricObject
{
	public:
		Collection();
		void addObject(GeometricObject *object);
        virtual void getBounds(float min[3], float max[3]) const;
		virtual void hit(Ray &ray, ShadeRec &sr) const;
	private:
		std::vector<GeometricObject*> m_objects;
		float m_min[3];
		float m_max[3];
};

#endif
