#ifndef __TORRANCE_SPARROW_H__
#define __TORRANCE_SPARROW_H__
#include "brdf.h"

class TorranceSparrowBRDF:public BRDF
{
	public:
		TorranceSparrowBRDF(float e, float ior);
		virtual float f(Vec3 &in, Vec3 &out) const;
		virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampling &sampling, int id) const;
	private:
		float blinnDistribution(const Vec3 &half) const;
		float g(const Vec3 &in, const Vec3 &out, const Vec3 &half) const;
		float m_exp;
		float m_ior;
		float m_INVTWOPI;
};

#endif
