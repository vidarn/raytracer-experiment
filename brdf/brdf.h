#ifndef __BRDF_H__
#define __BRDF_H__
#include "../utils/vec3.h"
#include "../shadeRec/shadeRec.h"
#include "../sampler/sampler.h"
#include <cmath>

enum BRDF_TYPE {BRDF_SPECULAR, BRDF_DIFFUSE};

class BRDF
{
	public:
		virtual void extractNormal(const void *data, Vec3 *normal) const = 0;
		virtual float f(const Vec3 &in, const Vec3 &out, const void *data) const = 0;
        virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampler &sampler, const void *data) const = 0;
        float fresnel(float cosI, float eta1, float eta2) const;
		Vec3 snell(Vec3 in, float eta) const;
		float absCosTheta(const Vec3 &vec)const{return fabsf(vec[2]);}
		BRDF_TYPE m_type;
};

#endif /* end of include guard: __BRDF_H__ */
