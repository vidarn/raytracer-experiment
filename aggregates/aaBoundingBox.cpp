#include "aaBoundingBox.h"
#include <cfloat>

AABoundingBox::AABoundingBox(GeometricObject *object)
	:m_object(object)
{
	m_transform = m_object->getTransform();
	Matrix4x4 tmpTransform;
	m_object->setTransform(tmpTransform);
	m_object->getBounds(m_min, m_max);
}

void AABoundingBox::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = m_min[i];
        max[i] = m_max[i];
    }
}

bool AABoundingBox::testBBox(Ray &ray) const
{
	float tmin;
	float tmax;
	float minTmin = DBL_MAX;
	float maxTmax = DBL_MIN;
	for (int i = 0; i < 3; i++) {
		if(ray.m_dir[i] == 0.0){
			if(ray.m_origin[i] < m_min[i] || ray.m_origin[i] > m_max[i]){
				return false;
			}
			continue;
		}
		float invD = 1.0/ray.m_dir[i];
		tmin = (m_min[i]-ray.m_origin[i])*invD;
		tmax = (m_max[i]-ray.m_origin[i])*invD;
		if(invD < 0.0){
			float tmp = tmin;
			tmin = tmax;
			tmax = tmp;
		}
		if(tmin < minTmin){
			minTmin = tmin;
		}
		if(tmax > maxTmax){
			maxTmax = tmax;
		}
	}
	return (minTmin < maxTmax && maxTmax > 0.0);
}

ShadeRec AABoundingBox::hit(Ray &ray) const
{
	ShadeRec sr;
	if(testBBox(ray)){
		sr = m_object->hit(ray);
	}
	return sr;
}
