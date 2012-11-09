#ifndef __BRDF_H__
#define __BRDF_H__
#include "../utils/vec3.h"
#include "../shadeRec/shadeRec.h"

class BRDF
{
	public:
		virtual float shade(ShadeRec &shadeRec, Vec3 &lightDir) const = 0;
};

#endif /* end of include guard: __BRDF_H__ */
