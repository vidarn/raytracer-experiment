#ifndef __SPECULAR_REFLECTION_H__
#define __SPECULAR_REFLECTION_H__
#include "brdf.h"

struct SpecularParams        { Vec3 N; float ior; };

class SpecularReflectionBRDF: public BRDF
{
	public:
		SpecularReflectionBRDF(){};
		virtual void extractNormal(const void *data, Vec3 *normal) const;
		virtual float f(const Vec3 &in, const Vec3 &out, const void *data) const;
        virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, const void *data) const;
};

#endif /* end of include guard: __SPECULAR_REFLECTION_H__ */
