#ifndef __LAMBERT_H__
#define __LAMBERT_H__
#include "brdf.h"

class LambertBRDF: public BRDF
{
	public:
		LambertBRDF(){};
		virtual float f(Vec3 &in, Vec3 &out) const;
        virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampling &sampling, int id) const;
	private:
};

#endif /* end of include guard: __LAMBERT_H__ */
