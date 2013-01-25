#include "light.h"


Light::Light(float strength):
    m_strength(strength)
{
}

RGBA Light::getLightColor(Vec3 &dir, Vec3 &point, Vec3 &samplePos, float *pdf)
{
    float a = computeStrength(dir,point, samplePos, pdf);
    return m_color*a;
}

Vec3 Light::getLightDirection(ShadeRec &sr, Sampler &sampler)
{
    return computeDirection(sr, sampler);
}
