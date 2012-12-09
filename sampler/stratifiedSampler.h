#ifndef __UNIFORM_SAMPLER_H__
#define __UNIFORM_SAMPLER_H__
#include "sampler.h"

class StratifiedSampler: public Sampler
{
    public:
		virtual void generate1DSequence(Vec3 *samples,int numSamples);
		virtual void generateSquareSequence(Vec3 *samples,int numSamples);
		virtual void generateDiskSequence(Vec3 *samples,int numSamples);
    private:
        float getSamplePos(int i, int num);
        void shuffle(Vec3 *samples, int numSamples);
};

#endif
