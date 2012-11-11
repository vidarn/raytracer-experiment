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

bool AABoundingBox::testBBox(Ray &ray, float &retTmin, float &retTmax) const
{
	float tmin;
	float tmax;
    retTmin = FLT_MAX;
	retTmax = -FLT_MAX;
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
		if(tmin < retTmin){
			retTmin = tmin;
		}
		if(tmax > retTmax){
			retTmax = tmax;
		}
	}
	return (retTmin < retTmax && retTmax > 0.0);
}

void AABoundingBox::hit(Ray &ray, ShadeRec &sr) const
{
    float tmin, tmax;
	if(testBBox(ray, tmin, tmax)){
		m_object->hit(ray,sr);
	}
}
