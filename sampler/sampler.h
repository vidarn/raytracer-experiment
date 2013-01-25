#ifndef __SAMPLER_H__
#define __SAMPLER_H__
#include <vector>
#include "../utils/vec3.h"

class Sampler
{
	public:
        float get1DSample();
        Vec3  getDiskSample();
        Vec3  getSquareSample();
        Vec3  getUniformHemisphereSample();
        Vec3  getCosineHemisphereSample();
    private:
        virtual float getSample() = 0;
};

#endif
