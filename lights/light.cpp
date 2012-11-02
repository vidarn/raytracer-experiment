#include "light.h"


Light::Light(double strength, Matrix4x4 transform)
{
    m_transform = transform;
    m_invTransform = transform.invert();
}

double Light::getLightStrength(Point pos)
{
    double result;
    result = m_strength*computeStrength(pos);
    return result;
}

Vec3 Light::getLightDirection(Point pos)
{
    pos = m_invTransform*pos;
    Vec3 dir = computeDirection(pos);
    dir = m_transform*dir;
    dir.normalize();
    return dir;
}
