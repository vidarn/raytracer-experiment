#include "phong.h"

float PhongBRDF::f(Vec3 &in, Vec3 &out) const
{
    Vec3 h = in + out;
    Vec3 n(0.0f,0.0f,1.0f);
    h.normalize();
    float val = h.dot(n);
    val = powf(val,m_p);
    val *= (m_p + 8.0f)/(8.0f * M_PI);
    return val;
}

float PhongBRDF::sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, int id) const
{
    Vec3 normal(0.0f, 0.0f, 1.0f);
    Vec3 refl(-in[0],-in[1],in[2]);
	*out = sampler.getUniformHemisphereSample();
	*pdf = 0.25f/M_PI;
    return 1.0f / out->dot(normal);
}
