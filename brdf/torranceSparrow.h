#ifndef __TORRANCE_SPARROW_H__
#define __TORRANCE_SPARROW_H__
#include "brdf.h"

struct TorranceSparrowParams { Vec3 N; float e; };

class TorranceSparrowBRDF:public BRDF
{
	public:
		TorranceSparrowBRDF(float ior);
		virtual void extractNormal(const void *data, Vec3 *normal) const;
		virtual float f(const Vec3 &in, const Vec3 &out, const void *data) const;
		virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, const void *data) const;
	private:
		float blinnDistribution(const Vec3 &half, float exp) const;
		float g(const Vec3 &in, const Vec3 &out, const Vec3 &half) const;
		float m_ior;
		float m_INVTWOPI;
};

#endif
