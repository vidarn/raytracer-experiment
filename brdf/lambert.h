#ifndef __LAMBERT_H__
#define __LAMBERT_H__
#include "brdf.h"

class Lambert: public BRDF
{
	public:
		Lambert(){};
		virtual float shade(ShadeRec &shadeRec, Vec3 &lightDir) const;
	private:
};

#endif /* end of include guard: __LAMBERT_H__ */
