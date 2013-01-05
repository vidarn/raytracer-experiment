#include "specularReflection.h"
#include <cstdlib>

float SpecularReflectionBRDF::f(Vec3 &in, Vec3 &out) const
{
	return 0.0f;
}

float SpecularReflectionBRDF::sample_f(Vec3 &in, Vec3 *out, Sampling &sampling, int id) const
{
    Vec3 normal(0.0f, 0.0f, 1.0f);
    *out = Vec3(-in.m_d[0],-in.m_d[1],in.m_d[2]);
    float eta1 = 1.0f;
    float eta2 = m_ior;
    float cosI = in.m_d[2];
    float sinI = sqrtf(1 - cosI*cosI);
    float cosT = eta1/eta2 * sinI;
    cosT = sqrtf(1.0f - cosT * cosT);
    return fresnel(cosI,cosT,eta1,eta2) / out->dot(normal);
}
