#ifndef __PHONG_H__
#define __PHONG_H__
#include "brdf.h"

class Phong
{
	public:
		Phong(){};
		virtual double shade(ShadeRec &shadeRec, Vec3 &lightDir) const;
	private:
};

#endif /* end of include guard: __PHONG_H__ */
