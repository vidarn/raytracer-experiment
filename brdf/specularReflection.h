#ifndef __SPECULAR_REFLECTION_H__
#define __SPECULAR_REFLECTION_H__
#include "brdf.h"

class SpecularReflectionBRDF: public BRDF
{
	public:
		SpecularReflectionBRDF(){};
		virtual float f(Vec3 &in, Vec3 &out) const;
        virtual float sample_f(Vec3 &in, Vec3 *out, Sampling &sampling, int id) const;
	private:
};

#endif /* end of include guard: __SPECULAR_REFLECTION_H__ */
