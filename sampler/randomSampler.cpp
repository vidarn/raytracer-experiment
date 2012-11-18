#include "randomSampler.h"
#include <cstdlib>

RandomSampler::RandomSampler()
{
}

Vec3 RandomSampler::getSquareSample()
{
    Vec3 sample;
    for(int i=0; i<2; i++){
        sample[i] = float(rand())/float(RAND_MAX);
    }
    return sample;
}
