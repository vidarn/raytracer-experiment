#include "brdf.h"

float BRDF::fresnel(float cosI, float eta1, float eta2) const
{
    float ret = 1.0f;
	if(eta1 > 0.0f && eta2 > 0.0f)
	{
		float sinI = sqrtf(1 - cosI*cosI);
		float cosT = eta1/eta2 * sinI;
		cosT = sqrtf(1.0f - cosT * cosT);
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
