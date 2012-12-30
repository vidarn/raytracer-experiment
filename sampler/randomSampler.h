#ifndef __RANDOM_SAMPLER_H__
#define __RANDOM_SAMPLER_H__
#include "sampler.h"
#include <cstdlib>

class RandomSampler: public Sampler
{
    public:
        RandomSampler(long int seed);
		virtual void generate1DSequence(Vec3 *samples,int numSamples);
		virtual void generateSquareSequence(Vec3 *samples,int numSamples);
		virtual void generateDiskSequence(Vec3 *samples,int numSamples);
		virtual void generateHemisphereSequence(Vec3 *samples,int numSamples);
		float getRandomNumber();
    private:
		struct drand48_data m_randData;
};

#endif
