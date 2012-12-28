#include "light.h"


Light::Light(float strength):
    m_strength(strength)
{
}

float Light::getLightStrength(Vec3 &pos)
{
    float result;
    result = m_strength*computeStrength(pos);
    return result;
}

Vec3 Light::getLightDirection(ShadeRec &sr, Sampling &sampling)
{
    return computeDirection(sr, sampling);
}
