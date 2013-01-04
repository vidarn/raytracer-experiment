#ifndef __BRDF_H__
#define __BRDF_H__
#include "../utils/vec3.h"
#include "../shadeRec/shadeRec.h"
#include "../sampler/sampling.h"

class BRDF
{
	public:
		virtual float f(Vec3 &in, Vec3 &out) const = 0;
        virtual float sample_f(Vec3 &in, Vec3 *out, Sampling &sampling, int id) const = 0;
        float fresnel(float cosI, float cosT, float eta1, float eta2) const;
};

#endif /* end of include guard: __BRDF_H__ */
