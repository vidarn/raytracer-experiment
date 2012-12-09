#include "randomSampler.h"
#include <cstdlib>

RandomSampler::RandomSampler()
{
}

void RandomSampler::generate1DSequence(Vec3 *samples,int numSamples)
{
    for(int i=0;i<numSamples;i++){
        samples[i][0] = float(rand())/float(RAND_MAX);
    }
}

void RandomSampler::generateSquareSequence(Vec3 *samples,int numSamples)
{
    for(int i=0; i<numSamples; i++){
        for(int j=0; j<2; j++){
            samples[i][j] = float(rand())/float(RAND_MAX);
        }
    }
}

void RandomSampler::generateDiskSequence(Vec3 *samples,int numSamples)
{
    for(int i=0;i<numSamples;i++){
        bool found = false;
        while(!found){
            for(int j=0; j<2; j++){
                samples[i][j] = float(rand())/float(RAND_MAX);
            }
            samples[i][0] = 1.0f - 2.0f*samples[i][0];
            samples[i][1] = 1.0f - 2.0f*samples[i][1];
            if(sqrtf(samples[i][0]*samples[i][0] + samples[i][1]*samples[i][1]) <= 1.0f){
                found = true;
            }
        }
    }
}
