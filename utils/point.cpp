#include "point.h"

Vec3 Point::operator-(Point &other)
{
	Vec3 tmp = m_vec;
	tmp -= other.m_vec;
	return tmp;
}

Point& Point::operator-=(Vec3 &vec)
{
	m_vec -= vec;
	return *this;
}

Point Point::operator-(Vec3 &vec)
{
	Point tmp = *this;
	tmp -= vec;
	return tmp;
}

Point& Point::operator+=(Vec3 &vec)
{
	m_vec += vec;
	return *this;
}

Point Point::operator+(Vec3 &vec)
{
	Point tmp = *this;
	tmp += vec;
	return tmp;
}

std::ostream& operator<<(std::ostream &out, Point &point)
{
	out << "P [" << point.x() << " , " << point.y() << " , " << point.z() << "]";
	return out;
}
