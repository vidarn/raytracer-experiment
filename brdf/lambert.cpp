#include "lambert.h"
#include <cstdlib>

float LambertBRDF::f(Vec3 &in, Vec3 &out) const
{
	return 1.0f/M_PI;
}

float LambertBRDF::sample_f(Vec3 &in, Vec3 *out, Sampling &sampling, int id) const
{
	*out = sampling.getHemisphereSample(id);
    return f(in,*out);
}
