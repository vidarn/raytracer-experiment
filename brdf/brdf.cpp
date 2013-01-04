#include "brdf.h"

float BRDF::fresnel(float cosI, float cosT, float eta1, float eta2) const
{
    float ret;
    float tmpA = eta1 * cosI;
    float tmpB = eta2 * cosT;
    float tmpRet = (tmpA - tmpB) / (tmpA + tmpB);
    ret = tmpRet*tmpRet;
    tmpB = eta1 * cosT;
    tmpA = eta2 * cosI;
    tmpRet = (tmpB - tmpA) / (tmpA + tmpB);
    ret += tmpRet*tmpRet;
    ret = ret * 0.5f;
    return ret;
}
