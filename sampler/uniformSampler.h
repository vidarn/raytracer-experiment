#ifndef __UNIFORM_SAMPLER_H__
#define __UNIFORM_SAMPLER_H__
#include "sampler.h"

class UniformSampler: public Sampler
{
    public:
        UniformSampler(int numSubdivisions);
		virtual Vec3 getSquareSample();
    private:
		int m_numSubdivisions;
		int m_numSamples;
		int m_currentSample;
};

#endif
