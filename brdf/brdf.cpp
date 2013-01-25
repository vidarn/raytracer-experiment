#include "brdf.h"

float BRDF::fresnel(float cosI, float eta1, float eta2) const
{
    float ret = 1.0f;
	if(eta1 > 0.0f && eta2 > 0.0f)
	{
		float sinI = sqrtf(std::max(0.0f, 1.0f - cosI*cosI));
		float sinT = eta1/eta2 * sinI;
		if(sinT >= 1.0f)
			return 1.0f;
		float cosT = sqrtf(std::max(0.0f,1.0f - sinT * sinT));
		float tmpA = eta1 * cosI;
		float tmpB = eta2 * cosT;
		float tmpRet = (tmpA - tmpB) / (tmpA + tmpB);
		ret = tmpRet*tmpRet;
		tmpB = eta1 * cosT;
		tmpA = eta2 * cosI;
		tmpRet = (tmpB - tmpA) / (tmpA + tmpB);
		ret += tmpRet*tmpRet;
		ret = ret * 0.5f;
	}
    return ret;
}

Vec3 BRDF::snell(Vec3 in, float eta) const
{
	Vec3 ret;
	float cosTheta = fabsf(in.m_d[2]);
	float sinTheta2 = std::max(0.0f, 1.0f - cosTheta * cosTheta);
	sinTheta2 = sinTheta2 * eta * eta;
	cosTheta = sqrtf(std::max(0.0f, 1.0f - sinTheta2));
	ret[0] = in[0] * -eta;
	ret[1] = in[1] * -eta;
	ret[2]  = cosTheta;
    return ret;
}
