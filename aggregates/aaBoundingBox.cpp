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

AABoundingBox::AABoundingBox()
{
    clear();
}

void AABoundingBox::extend(GeometricObject *object)
{
	float min[3];
	float max[3];
	object->getBounds(min,max);
	for(int i = 0; i < 3; i++){
		m_min[i] = min[i]<m_min[i]?min[i]:m_min[i];
		m_max[i] = max[i]>m_max[i]?max[i]:m_max[i];
	}
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

int AABoundingBox::getMaximumExtent()
{
	int maxAxis = 0;
	for(int i=1; i<3;i ++){
		maxAxis = (m_max[i] - m_min[i]) > (m_max[maxAxis] - m_min[maxAxis]) ? i : maxAxis;
	}
	return maxAxis;
}

void AABoundingBox::clear()
{
	m_object = NULL;
	for(int i = 0; i < 3; i++){
		m_min[i] =  FLT_MAX;
		m_max[i] = -FLT_MAX;
	}
}

float AABoundingBox::surfaceArea()
{
    Vec3 d(m_max[0]-m_min[0],m_max[1]-m_min[1],m_max[2]-m_min[2]);
    return 2.0f * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
}

Vec3 AABoundingBox::diagonalVec()
{
    Vec3 d(m_max[0]-m_min[0],m_max[1]-m_min[1],m_max[2]-m_min[2]);
    return d;
}
