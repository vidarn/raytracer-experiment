#ifndef __LAMBERT_H__
#define __LAMBERT_H__
#include "brdf.h"

struct DiffuseParams  { Vec3 N; };

class LambertBRDF: public BRDF
{
	public:
		virtual void extractNormal(const void *data, Vec3 *normal) const;
		LambertBRDF(){m_type = BRDF_DIFFUSE;};
		virtual float f(const Vec3 &in, const Vec3 &out, const void *data) const;
        virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, const void *data) const;
	private:
};

#endif /* end of include guard: __LAMBERT_H__ */
