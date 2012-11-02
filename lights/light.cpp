#include "light.h"


Light::Light(double strength, Matrix4x4 transform)
{
    std::cout << transform;
    m_transform = transform;
    m_invTransform = transform.invert();
    std::cout << m_invTransform;
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
    //std::cout << dir << std::endl;
    dir = m_transform*dir;
    //std::cout << dir << std::endl;
    dir.normalize();
    //std::cout << dir << std::endl;
    //std::cout << "-----\n";
    return dir;
}
