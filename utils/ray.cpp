#include "ray.h"

std::ostream& operator<<(std::ostream &out, Ray &vec)
{
	out << "R [" << vec.m_origin << " , " << vec.m_dir << "]";
	return out;
}
