#include "rgba.h"

RGBA::RGBA(std::ifstream &stream)
{
	for(int i=0;i<4;i++){
		stream.read( reinterpret_cast<char*>( &(m_col[i]) ), sizeof m_col[i] );
	}
}

RGBA& RGBA::operator+=(const RGBA &other)
{
	m_col[0] += other.m_col[0];
	m_col[1] += other.m_col[1];
	m_col[2] += other.m_col[2];
	m_col[3] += other.m_col[3];
	return *this;
}

RGBA RGBA::operator+(const RGBA &other)
{
	RGBA tmp = *this;
	tmp += other;
	return tmp;
}

RGBA& RGBA::operator-=(const RGBA &other)
{
	m_col[0] -= other.m_col[0];
	m_col[1] -= other.m_col[1];
	m_col[2] -= other.m_col[2];
	m_col[3] -= other.m_col[3];
	return *this;
}

RGBA RGBA::operator-(const RGBA &other)
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

RGBA RGBA::operator*(const float &other) const
{
	RGBA tmp = *this;
	tmp *= other;
	return tmp;
}

RGBA& RGBA::operator*=(const RGBA &other)
{
	m_col[0] *= other.m_col[0];
	m_col[1] *= other.m_col[1];
	m_col[2] *= other.m_col[2];
	m_col[3] *= other.m_col[3];
	return *this;
}

RGBA RGBA::operator*(const RGBA &other) const
{
	RGBA tmp = *this;
	tmp *= other;
	return tmp;
}

RGBA& RGBA::operator/=(const RGBA &other)
{
	m_col[0] /= other.m_col[0];
	m_col[1] /= other.m_col[1];
	m_col[2] /= other.m_col[2];
	m_col[3] /= other.m_col[3];
	return *this;
}

RGBA RGBA::operator/(const RGBA &other) const
{
	RGBA tmp = *this;
	tmp /= other;
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
