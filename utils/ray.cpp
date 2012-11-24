#include "ray.h"

Vec3 Ray::getPointAtPos(float t)
{
	Vec3 tmp = m_dir;
	tmp *= t;
	return m_origin + tmp;
}

std::ostream& operator<<(std::ostream &out, Ray &vec)
{
	out << "R [" << vec.m_origin << " , " << vec.m_dir << "]";
	return out;
}

void Ray::computePlucker()
{
    Vec3 p = m_origin;
    Vec3 q = m_origin+m_dir;
    m_plucker[0] = p[0] * q[1] - q[0] * p[1];
    m_plucker[1] = p[0] * q[2] - q[0] * p[2];
    m_plucker[2] = p[0] - q[0];
    m_plucker[3] = p[1] * q[2] - q[1] * p[2];
    m_plucker[4] = p[2] - q[2];
    m_plucker[5] = q[1] - p[1];
}
