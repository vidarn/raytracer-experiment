#ifndef __RANDOM_SAMPLER_H__
#define __RANDOM_SAMPLER_H__
#include "sampler.h"

class RandomSampler: public Sampler
{
    public:
        RandomSampler();
		virtual Vec3 getSquareSample();
    private:
};

#endif
