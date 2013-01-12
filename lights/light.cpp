#include "light.h"


Light::Light(float strength):
    m_strength(strength)
{
}

RGBA Light::getLightColor(Vec3 &pos)
{
    return m_color*computeStrength(pos);
}

Vec3 Light::getLightDirection(ShadeRec &sr, Sampling &sampling)
{
    return computeDirection(sr, sampling);
}
