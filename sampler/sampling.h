#ifndef __SAMPLING_H__
#define __SAMPLING_H__
#include "sampler.h"
#include <vector>

class Sampling
{
    public:
        Sampling(int numSamples, int num1DSequences, int numSquareSequences, int numDiskSequences, Sampler* sampler);
        void increaseIndex();
        Vec3 get1DSample(int id);
        Vec3 getDiskSample(int id);
        Vec3 getSquareSample(int id);

    private:
        int m_currentIndex;
        std::vector<Vec3*>m_1DSequences;
        std::vector<Vec3*>m_squareSequences;
        std::vector<Vec3*>m_diskSequences;
};

#endif
