#include "sampling.h"


Sampling::Sampling(int numSamples, int num1DSequences, int numSquareSequences, int numDiskSequences, Sampler* sampler)
{
    for(int i=0;i<num1DSequences;i++){
        Vec3 *samples = new Vec3[numSamples];
        sampler->generate1DSequence(samples,numSamples);
        m_1DSequences.push_back(samples);
    }
    for(int i=0;i<numSquareSequences;i++){
        Vec3 *samples = new Vec3[numSamples];
        sampler->generateSquareSequence(samples,numSamples);
        m_squareSequences.push_back(samples);
    }
    for(int i=0;i<numDiskSequences;i++){
        Vec3 *samples = new Vec3[numSamples];
        sampler->generateDiskSequence(samples,numSamples);
        m_diskSequences.push_back(samples);
    }
    m_currentIndex = 0;
}

void Sampling::increaseIndex(){
    m_currentIndex++;
}

Vec3 Sampling::get1DSample(int id)
{
    return m_1DSequences[id][m_currentIndex];
}

Vec3 Sampling::getDiskSample(int id)
{
    return m_diskSequences[id][m_currentIndex];
}

Vec3 Sampling::getSquareSample(int id)
{
    return m_squareSequences[id][m_currentIndex];
}
