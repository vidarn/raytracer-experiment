#include "area.h"
#include <cstdlib>

AreaLight::AreaLight(std::ifstream &stream, Matrix4x4 transform)
{
	m_transform = transform;
    stream.read( reinterpret_cast<char *>( &m_strength), sizeof m_strength);
	stream.read( reinterpret_cast<char *>( &(m_size[0])), sizeof m_size[0]);
    stream.read( reinterpret_cast<char *>( &(m_size[1])), sizeof m_size[1]);
    //m_color = RGBA(stream);
    m_color = RGBA(1.0f);
	m_color[0] = pow(m_color[0],2.2f);
	m_color[1] = pow(m_color[1],2.2f);
	m_color[2] = pow(m_color[2],2.2f);
    m_normal = Vec3(0.0f,0.0f,1.0f);
    m_transform.multDirMatrix(m_normal, m_normal);
    m_area = m_size[0] * m_size[1];
}

float AreaLight::computeStrength(Vec3 &dir, Vec3 &point, Vec3 &samplePos, float *pdf)
{
    *pdf =  (samplePos - point).length2()/ (fabsf(m_normal.dot(dir)) * m_area);
    if(dir.dot(m_normal) > 0.0f)
        return m_strength;
    else
        return 0.0f;
}

Vec3 AreaLight::computeDirection(ShadeRec &sr, Sampler &sampler)
{
	Vec3 point = sampler.getSquareSample();
    point[0] = m_size[0]*(point[0] - 0.5f);
    point[1] = m_size[1]*(point[1] - 0.5f);
	m_transform.multVecMatrix(point,point);
    Vec3 ret = (point - sr.m_hitPos);
    ret.normalize();
	return ret;
}

Ray AreaLight::getRay(Sampler &sampler)
{
	Vec3 point = sampler.getSquareSample();
    point[0] = m_size[0]*(point[0] - 0.5f);
    point[1] = m_size[1]*(point[1] - 0.5f);
	m_transform.multVecMatrix(point,point);
	Vec3 dir(0.0f,0.0f,-1.0f);
	dir = sampler.getUniformHemisphereSample();
    m_transform.multDirMatrix(dir, dir);
	dir *= -1.0f;
	Ray ray(point,dir,false);
	ray.computePlucker();
	ray.m_depth = 2;
	return ray;
}
