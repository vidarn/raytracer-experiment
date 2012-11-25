#ifndef __SAMPLER_H__
#define __SAMPLER_H__
#include <vector>
#include "../utils/vec3.h"

class Sampler
{
	public:
		virtual Vec3 getSquareSample() = 0;
		virtual Vec3 getDiskSample() = 0;
};

#endif
