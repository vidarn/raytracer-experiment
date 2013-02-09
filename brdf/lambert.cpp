#include "lambert.h"
#include <cstdlib>

void LambertBRDF::extractNormal(const void *data, Vec3 *normal) const
{
    //struct DiffuseParams *params = (struct DiffuseParams *)data;
    //(*normal)[0] = params->N[0];
    //(*normal)[1] = params->N[1];
    //(*normal)[2] = params->N[2];
}

float LambertBRDF::f(const Vec3 &in, const Vec3 &out, const void *data) const
{
	return 1.0f/M_PI;
}

float LambertBRDF::sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, const void *data) const
{
	*out = sampler.getCosineHemisphereSample();
	*pdf = absCosTheta(*out)/M_PI;
    return f(in,*out, data);
}
