#include "lambert.h"
#include <cstdlib>

float LambertBRDF::f(const Vec3 &in, const Vec3 &out) const
{
	return 1.0f/M_PI;
}

float LambertBRDF::sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, int id) const
{
	*out = sampler.getCosineHemisphereSample();
	*pdf = absCosTheta(*out)/M_PI;
    return f(in,*out);
}
