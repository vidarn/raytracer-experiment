#ifndef __RANDOM_SAMPLER_H__
#define __RANDOM_SAMPLER_H__
#include "sampler.h"

class RandomSampler: public Sampler
{
    public:
        RandomSampler();
		virtual void generate1DSequence(Vec3 *samples,int numSamples);
		virtual void generateSquareSequence(Vec3 *samples,int numSamples);
		virtual void generateDiskSequence(Vec3 *samples,int numSamples);
		virtual void generateHemisphereSequence(Vec3 *samples,int numSamples);
    private:
};

#endif
