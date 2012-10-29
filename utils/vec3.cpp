#include "vec3.h"

double Vec3::dot(Vec3 &other)
{
	return m_d[0]*other.m_d[0] + m_d[1]*other.m_d[1] + m_d[2]*other.m_d[2];
}

Vec3 Vec3::cross(Vec3 &rhs)
{
	double result[3];
	result[0] = y()*rhs.z() - z()*rhs.y();
	result[1] = z()*rhs.x() - x()*rhs.z();
	result[2] = x()*rhs.y() - y()*rhs.x();
	return Vec3(result);
}

void Vec3::normalize()
{
	double mag = magnitude();
	if(mag != 0){
		double invmag = 1.0/mag;
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

Vec3& Vec3::operator+=(Vec3 &other)
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

Vec3& Vec3::operator-=(Vec3 &other)
{
	m_d[0] -= other.m_d[0];
	m_d[1] -= other.m_d[1];
	m_d[2] -= other.m_d[2];
	return *this;
}

Vec3 Vec3::operator-(Vec3 &other)
{
	Vec3 tmp = *this;
	tmp -= other;
	return tmp;
}

Vec3& Vec3::operator*=(double &other)
{
	m_d[0] *= other;
	m_d[1] *= other;
	m_d[2] *= other;
	return *this;
}

Vec3 Vec3::operator*(double &other)
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
