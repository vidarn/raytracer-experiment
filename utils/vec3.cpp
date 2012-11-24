#include "vec3.h"

float Vec3::dot(const Vec3 &other) const
{
	return m_d[0]*other.m_d[0] + m_d[1]*other.m_d[1] + m_d[2]*other.m_d[2];
}

Vec3 Vec3::cross(const Vec3 &rhs) const
{
	float result[3];
	result[0] = m_d[1]*rhs.m_d[2] - m_d[2]*rhs.m_d[1];
	result[1] = m_d[2]*rhs.m_d[0] - m_d[0]*rhs.m_d[2];
	result[2] = m_d[0]*rhs.m_d[1] - m_d[1]*rhs.m_d[0];
	return Vec3(result);
}

Vec3 Vec3::reflect(Vec3 &other)
{
	Vec3 thisVec = Vec3(m_d);
	float val = thisVec.dot(other);
	val *= 2.0;
	Vec3 result = thisVec*val - other;
	return result;
}

void Vec3::normalize()
{
	float mag = magnitude();
	if(mag != 0){
		float invmag = 1.0/mag;
		m_d[0] = m_d[0]*invmag;
		m_d[1] = m_d[1]*invmag;
		m_d[2] = m_d[2]*invmag;
	}
}

Vec3 Vec3::getNormalized()
{
	Vec3 tmp(m_d);
	tmp.normalize();
	return tmp;
}

void Vec3::invert()
{
	for (int i = 0; i < 3; i++) {
		m_d[i] = - m_d[i];
	}
}

Vec3& Vec3::operator+=(const Vec3 &other)
{
	m_d[0] += other.m_d[0];
	m_d[1] += other.m_d[1];
	m_d[2] += other.m_d[2];
	return *this;
}

Vec3 Vec3::operator+(Vec3 &other)
{
	Vec3 tmp = *this;
	tmp += other;
	return tmp;
}

Vec3& Vec3::operator-=(const Vec3 &other)
{
	m_d[0] -= other.m_d[0];
	m_d[1] -= other.m_d[1];
	m_d[2] -= other.m_d[2];
	return *this;
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
	Vec3 tmp = *this;
	tmp -= other;
	return tmp;
}

Vec3& Vec3::operator*=(const float &other)
{
	m_d[0] *= other;
	m_d[1] *= other;
	m_d[2] *= other;
	return *this;
}

Vec3 Vec3::operator*(const float &other) const
{
	Vec3 tmp = *this;
	tmp *= other;
	return tmp;
}

std::ostream& operator<<(std::ostream &out, Vec3 &vec)
{
	out << "V [" << vec.x() << " , " << vec.y() << " , " << vec.z() << "]";
	return out;
}
