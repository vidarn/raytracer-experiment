#include "specularReflection.h"
#include <cstdlib>

float SpecularReflectionBRDF::f(const Vec3 &in, const Vec3 &out) const
{
	return 0.0f;
}

float SpecularReflectionBRDF::sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampling &sampling, int id) const
{
    Vec3 normal(0.0f, 0.0f, 1.0f);
    *out = Vec3(-in.m_d[0],-in.m_d[1],in.m_d[2]);
    float eta1 = 1.0f;
    float eta2 = m_ior;
    float cosI = in.m_d[2];
    *pdf = 1.0f;
    return fresnel(cosI,eta1,eta2) / out->dot(normal);
}
