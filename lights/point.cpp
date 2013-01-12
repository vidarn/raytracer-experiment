#include "point.h"

PointLight::PointLight(std::ifstream &stream, Matrix4x4 transform)
{
    m_point = Vec3(0.0f,0.0f,0.0f);
    m_point = transform.multPoint(m_point);
    stream.read( reinterpret_cast<char *>( &m_strength), sizeof m_strength);
}

PointLight::PointLight(float strength, Matrix4x4 transform)
:Light(strength)
{
    m_point = Vec3(0.0f,0.0f,0.0f);
    m_point = transform.multPoint(m_point);
}

float PointLight::computeStrength(Vec3 &pos)
{
	return m_strength;
}

Vec3 PointLight::computeDirection(ShadeRec &sr, Sampling &sampling)
{
    Vec3 ret = (m_point - sr.m_hitPos);
    ret.normalize();

	return ret;
}

Ray PointLight::getRay(Sampling &sampling)
{
	Vec3 dir(0.0f,0.0f,1.0f);
	return Ray(m_point,dir,false);
}
