#ifndef __RANDOM_SAMPLER_H__
#define __RANDOM_SAMPLER_H__
#include "sampler.h"
#include <cstdlib>

class RandomSampler: public Sampler
{
    public:
        RandomSampler(long int seed);
    private:
        virtual float getSample();
		struct drand48_data m_randData;
};

#endif
