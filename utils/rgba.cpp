#include "rgba.h"

RGBA& RGBA::operator+=(RGBA &other)
{
	m_col[0] += other.m_col[0];
	m_col[1] += other.m_col[1];
	m_col[2] += other.m_col[2];
	m_col[3] += other.m_col[3];
	return *this;
}

RGBA RGBA::operator+(RGBA &other)
{
	RGBA tmp = *this;
	tmp += other;
	return tmp;
}

RGBA& RGBA::operator-=(RGBA &other)
{
	m_col[0] -= other.m_col[0];
	m_col[1] -= other.m_col[1];
	m_col[2] -= other.m_col[2];
	m_col[3] -= other.m_col[3];
	return *this;
}

RGBA RGBA::operator-(RGBA &other)
{
	RGBA tmp = *this;
	tmp -= other;
	return tmp;
}

RGBA& RGBA::operator*=(double &other)
{
	m_col[0] *= other;
	m_col[1] *= other;
	m_col[2] *= other;
	m_col[3] *= other;
	return *this;
}

RGBA RGBA::operator*(double &other) const
{
	RGBA tmp = *this;
	tmp *= other;
	return tmp;
}
std::ostream& operator<<(std::ostream &out, RGBA &rgba)
{
	out << "RGBA [" << rgba.r() << " , " << rgba.g() << " , " << rgba.b() << " , " << rgba.a() << "]";
	return out;
}
