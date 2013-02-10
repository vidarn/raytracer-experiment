#include "torranceSparrow.h"
#include <OpenEXR/ImathVecAlgo.h>

TorranceSparrowBRDF::TorranceSparrowBRDF(float ior)
{
	m_ior = ior;
	m_INVTWOPI = 0.5f/M_PI;
	m_type = BRDF_SPECULAR;
}

void TorranceSparrowBRDF::extractNormal(const void *data, Vec3 *normal) const
{
    //struct TorranceSparrowParams *params = (struct TorranceSparrowParams *)data;
}

float TorranceSparrowBRDF::f(const Vec3 &in, const Vec3 &out, const void *data) const
{
    struct TorranceSparrowParams *params = (struct TorranceSparrowParams *)data;
    float exp = params->e;
	float ret;
	float cosThetaI = absCosTheta(in );
	float cosThetaO = absCosTheta(out);
	if(cosThetaI == 0.0f || cosThetaO == 0.0f)
		return 0.0f;
	Vec3 half = in + out;
	half.normalize();
	float cosThetaH = half.dot(in);
	float f = fresnel(cosThetaH,1.0,m_ior);
	ret = blinnDistribution(half, exp) * f / (4.0f * cosThetaI * cosThetaO);
	ret *= g(in,out,half);
	return ret;
}

float TorranceSparrowBRDF::sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, const void *data) const
{
    struct TorranceSparrowParams *params = (struct TorranceSparrowParams *)data;
    float exp = params->e;
	Vec3 r = sampler.getSquareSample();
    if(r[0] < 0.0f){
        r[0] *= -1.0f;
    }
	float cosTheta = powf(r[0], 1.0f/(exp+1));
	float sinTheta = sqrtf(std::max(0.0f, 1.0f - cosTheta*cosTheta));
	float phi = r[1] * 2.0f * M_PI;
	Vec3 half = Vec3(sinTheta * cosf(phi), sinTheta * sinf(phi), cosTheta);
	*out = Imath::reflect(half, in);
	*pdf = ((exp + 1) * powf(cosTheta, exp))/(2.0f *  M_PI * 4.0f * in.dot(half));
    if((*out)[2] < 0.0f){
        *pdf = 0.0f;
    }
    return f(in,*out, data);
}

float TorranceSparrowBRDF::blinnDistribution(const Vec3 &half, float exp) const
{
	float cosTheta = absCosTheta(half);
	return (exp + 2) * m_INVTWOPI * powf(cosTheta,exp);
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
