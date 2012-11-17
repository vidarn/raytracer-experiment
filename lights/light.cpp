#include "light.h"


Light::Light(float strength, Matrix4x4 transform)
{
    m_transform = transform;
    m_invTransform = transform.invert();
}

float Light::getLightStrength(Vec3 pos)
{
    float result;
    result = m_strength*computeStrength(pos);
    return result;
}

Vec3 Light::getLightDirection(Vec3 pos)
{
    //pos = m_invTransform*pos;
    Vec3 dir = computeDirection(pos);
    dir = m_transform.multVec3(dir);
    dir.normalize();
    return dir;
}
