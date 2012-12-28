#include "area.h"
#include <cstdlib>

AreaLight::AreaLight(std::ifstream &stream, Matrix4x4 transform)
{
	m_transform = transform;
    stream.read( reinterpret_cast<char *>( &m_strength), sizeof m_strength);
	stream.read( reinterpret_cast<char *>( &(m_size[0])), sizeof m_size[0]);
    stream.read( reinterpret_cast<char *>( &(m_size[1])), sizeof m_size[1]);
}

float AreaLight::computeStrength(Vec3 &pos)
{
	return m_strength;
}

Vec3 AreaLight::computeDirection(ShadeRec &sr, Sampling &sampling)
{
	Vec3 point = sampling.getSquareSample(2);
    point[0] = m_size[0]*(point[0] - 0.5f);
    point[1] = m_size[1]*(point[1] - 0.5f);
	point = m_transform.multPoint(point);
    Vec3 ret = (point - sr.m_hitPos);
    ret.normalize();
	return ret;
}
