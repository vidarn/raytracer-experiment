#include "collection.h"
#include <cfloat>

Collection::Collection()
{
    for (int i = 0; i < 3; i++) {
        m_min[i] = DBL_MAX;
        m_max[i] = DBL_MIN;
    }
}

void Collection::addObject(GeometricObject *object)
{
	m_objects.push_back(object);
	float min[3];
	float max[3];
	object->getBounds(min,max);
	for (int i = 0; i < 3; i++) {
		if(min[i] < m_min[i]){
			m_min[i] = min[i];
		}
		if(max[i] > m_max[i]){
			m_max[i] = max[i];
		}
	}
}

void Collection::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = m_min[i];
        max[i] = m_max[i];
    }
}

ShadeRec Collection::hit(Ray &ray) const
{
	ShadeRec ret;
	float t = DBL_MAX;
	for (int i = 0; i < m_objects.size(); i++) {
		Point tmpPoint = m_objects[i]->getTransform()*ray.m_origin;
		Vec3 tmpDir = m_objects[i]->getTransform()*ray.m_dir;
		Ray tmpRay(tmpPoint,tmpDir);

		ShadeRec sr = m_objects[i]->hit(tmpRay);
		if(sr.getHit()){
			if(sr.getHitT() < t){
				t = sr.getHitT();
				ret = sr;
			}
		}
	}
	return ret;
}
