#include "directional.h"

Directional::Directional(float strength, Matrix4x4 transform)
:Light(strength)
{
    m_dir = Vec3(0.0f,0.0f,1.0f);
    m_dir = transform.multVec3(m_dir);
}

float Directional::computeStrength(Vec3 &pos)
{
	return 1.0;
}

Vec3 Directional::computeDirection(Vec3 &pos)
{
	return m_dir;
}
