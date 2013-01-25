#ifndef __LAMBERT_H__
#define __LAMBERT_H__
#include "brdf.h"

class LambertBRDF: public BRDF
{
	public:
		LambertBRDF(){m_type = BRDF_DIFFUSE;};
		virtual float f(const Vec3 &in, const Vec3 &out) const;
        virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, int id) const;
	private:
};

#endif /* end of include guard: __LAMBERT_H__ */
