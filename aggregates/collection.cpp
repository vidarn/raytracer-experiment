#include "collection.h"
#include <cfloat>

Collection::Collection()
{
    for (int i = 0; i < 3; i++) {
        m_min[i] = FLT_MAX;
        m_max[i] = -FLT_MAX;
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

void Collection::hit(Ray &ray, ShadeRec &sr) const
{
	float t = FLT_MAX;
	for (int i = 0; i < m_objects.size(); i++) {
        ShadeRec tmp;
		Vec3 tmpPoint = m_objects[i]->getTransform().multPoint(ray.m_origin);
		Vec3 tmpDir   = m_objects[i]->getTransform().multVec3(ray.m_dir);
		Ray tmpRay(tmpPoint,tmpDir);

        m_objects[i]->hit(tmpRay, tmp);
		if(tmp.getHit()){
			if(tmp.getHitT() < t){
				t = tmp.getHitT();
				sr = tmp;
			}
		}
	}
}
