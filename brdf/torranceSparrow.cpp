#include "torranceSparrow.h"

TorranceSparrowBRDF::TorranceSparrowBRDF(float e, float ior)
{
	m_exp = e;
	m_ior = ior;
	m_INVTWOPI = 0.5f/M_PI;
}

float TorranceSparrowBRDF::f(const Vec3 &in, const Vec3 &out) const
{
	float ret;
	float cosThetaI = absCosTheta(in );
	float cosThetaO = absCosTheta(out);
	if(cosThetaI == 0.0f || cosThetaO == 0.0f)
		return 0.0f;
	Vec3 half = in + out;
	half.normalize();
	float cosThetaH = half.dot(in);
	float f = fresnel(cosThetaH,1.0,m_ior);
	ret = blinnDistribution(half) * f / (4.0f * cosThetaI * cosThetaO);
	ret *= g(in,out,half);
	return ret;
}

float TorranceSparrowBRDF::sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampling &sampling, int id) const
{
	Vec3 r = sampling.getHemisphereSample(id);
	float cosTheta = powf(r.m_d[0], 1.0f/(m_exp+1));
	float sinTheta = sqrtf(std::max(0.0f, 1.0f - cosTheta*cosTheta));
	float phi = r.m_d[1] * 2.0f * M_PI;
	Vec3 half = Vec3(sinTheta * cosf(phi), sinTheta * sinf(phi), cosTheta);
	*out = half.reflect(in);
	*pdf = ((m_exp + 1) * powf(cosTheta, m_exp))/(2.0f *  M_PI * 4.0f * in.dot(half));

    return f(in,*out);
}

float TorranceSparrowBRDF::blinnDistribution(const Vec3 &half) const
{
	float cosTheta = absCosTheta(half);
	return (m_exp + 2) * m_INVTWOPI * powf(cosTheta,m_exp);
}

float TorranceSparrowBRDF::g(const Vec3 &in, const Vec3 &out, const Vec3 &half) const
{
	float NdotH = absCosTheta(half);
	float NdotO = absCosTheta(out );
	float NdotI = absCosTheta(in  );
	float OdotH = fabsf(out.dot(in));
	return std::min( 1.0f, std::min( 2.0f * NdotH * NdotO / OdotH,
						   2.0f * NdotH * NdotI / OdotH));
}
