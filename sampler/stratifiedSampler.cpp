#include "stratifiedSampler.h"
#include <cstdlib>
#include <algorithm>

void StratifiedSampler::generate1DSequence(Vec3 *samples,int numSamples)
{
    for(int i=0;i<numSamples;i++){
        samples[i][0] = getSamplePos(i,numSamples);
    }
    shuffle(samples, numSamples);
}

void StratifiedSampler::generateSquareSequence(Vec3 *samples,int numSamples)
{
    int numSquared = floor(sqrtf(numSamples));
    assert(numSamples == numSquared*numSquared);
    for(int y=0; y<numSquared; y++){
        for(int x=0; x<numSquared; x++){
            samples[x+y*numSquared][0] = getSamplePos(x,numSquared);
            samples[x+y*numSquared][1] = getSamplePos(y,numSquared);
        }
    }
    shuffle(samples, numSamples);
}

void StratifiedSampler::generateDiskSequence(Vec3 *samples,int numSamples)
{
    int numSquared = floor(sqrtf(numSamples));
    assert(numSamples == numSquared*numSquared);
    for(int y=0; y<numSquared; y++){
        for(int x=0; x<numSquared; x++){
			int i = x+y*numSquared;
            samples[i][0] = getSamplePos(x,numSquared);
            samples[i][1] = getSamplePos(y,numSquared);
			float r = sqrtf(samples[i][0]);
			float theta = 2.0f * M_PI * samples[i][1];
			samples[i][0] = r * cosf(theta);
			samples[i][1] = r * sinf(theta);
		}
    }
    shuffle(samples, numSamples);
}

void StratifiedSampler::generateHemisphereSequence(Vec3 *samples,int numSamples)
{
    generateSquareSequence(samples,numSamples);
}

void StratifiedSampler::shuffle(Vec3 *samples, int numSamples)
{
    std::random_shuffle(samples,&(samples[numSamples-1]));
}

float StratifiedSampler::getSamplePos(int i, int num)
{
    float val =  float(i)/float(num);
    val += float(rand())/float(RAND_MAX)/float(num);
    return val;
}
