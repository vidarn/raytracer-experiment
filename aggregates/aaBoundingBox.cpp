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

void AABoundingBox::getBounds(double min[3], double max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = m_min[i];
        max[i] = m_max[i];
    }
}

bool AABoundingBox::testBBox(Ray &ray) const
{
	double tmin;
	double tmax;
	double minTmin = DBL_MAX;
	double maxTmax = DBL_MIN;
	for (int i = 0; i < 3; i++) {
		if(ray.m_dir[i] == 0.0){
			if(ray.m_origin[i] < m_min[i] || ray.m_origin[i] > m_max[i]){
				return false;
			}
			continue;
		}
		double invD = 1.0/ray.m_dir[i];
		tmin = (m_min[i]-ray.m_origin[i])*invD;
		tmax = (m_max[i]-ray.m_origin[i])*invD;
		if(invD < 0.0){
			double tmp = tmin;
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
