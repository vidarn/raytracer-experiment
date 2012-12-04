#include "rgba.h"

RGBA& RGBA::operator+=(const RGBA &other)
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

RGBA& RGBA::operator*=(const float &other)
{
	m_col[0] *= other;
	m_col[1] *= other;
	m_col[2] *= other;
	m_col[3] *= other;
	return *this;
}

RGBA RGBA::operator*(float &other) const
{
	RGBA tmp = *this;
	tmp *= other;
	return tmp;
}

void RGBA::clamp(float val)
{
	for (int i = 0; i < 4; i++) {
		m_col[i] = m_col[i] < val ? m_col[i] : val;
	}
}

float RGBA::value()
{
	return (m_col[0] + m_col[1] + m_col[2] ) / 3.0f;
}

std::ostream& operator<<(std::ostream &out, RGBA &rgba)
{
	out << "RGBA [" << rgba.r() << " , " << rgba.g() << " , " << rgba.b() << " , " << rgba.a() << "]";
	return out;
}
