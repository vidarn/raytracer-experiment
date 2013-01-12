#ifndef __BRDF_H__
#define __BRDF_H__
#include "../utils/vec3.h"
#include "../shadeRec/shadeRec.h"
#include "../sampler/sampling.h"
#include <cmath>

class BRDF
{
	public:
		virtual float f(const Vec3 &in, const Vec3 &out) const = 0;
        virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampling &sampling, int id) const = 0;
        float fresnel(float cosI, float eta1, float eta2) const;
		float absCosTheta(const Vec3 &vec)const{return fabsf(vec.m_d[2]);}
};

#endif /* end of include guard: __BRDF_H__ */
