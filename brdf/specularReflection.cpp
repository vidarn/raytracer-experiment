#include "specularReflection.h"
#include <cstdlib>

void SpecularReflectionBRDF::extractNormal(const void *data, Vec3 *normal) const
{
    struct SpecularParams *params = (struct SpecularParams *)data;
}

float SpecularReflectionBRDF::f(const Vec3 &in, const Vec3 &out, const void *data) const
{
	return 0.0f;
}

float SpecularReflectionBRDF::sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, const void *data) const
{
    struct SpecularParams *params = (struct SpecularParams *)data;
    Vec3 normal(0.0f, 0.0f, 1.0f);
    *out = Vec3(-in[0],-in[1],in[2]);
    float eta1 = 1.0f;
    float eta2 = params->ior;
    float cosI = std::max(0.0f,in[2]);
    *pdf = 1.0f;
    float a =  (*out)[2];
	if(a > 0.001f){
		return fresnel(cosI,eta1,eta2);
	}
	else
		return 0.0f;
}
