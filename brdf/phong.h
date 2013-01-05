#ifndef __PHONG_H__
#define __PHONG_H__
#include "brdf.h"

class PhongBRDF: public BRDF
// Blinn-Phong Rather
{
	public:
		PhongBRDF(float p):m_p(p){};
		virtual float f(Vec3 &in, Vec3 &out) const;
        virtual float sample_f(Vec3 &in, Vec3 *out, float *pdf, Sampling &sampling, int id) const;
	private:
        float m_p;
};

#endif
