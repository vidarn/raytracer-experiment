#include "ray.h"

Point Ray::getPointAtPos(double t)
{
	Vec3 tmp = m_dir.getNormalized();
	tmp *= t;
	return m_origin + tmp;
}

std::ostream& operator<<(std::ostream &out, Ray &vec)
{
	out << "R [" << vec.m_origin << " , " << vec.m_dir << "]";
	return out;
}
