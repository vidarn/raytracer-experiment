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

Vec3 RandomSampler::getDiskSample()
{
	bool found = false;
	Vec3 sample;
	while(!found){
		sample = getSquareSample();
		sample[0] = 1.0f - 2.0f*sample[0];
		sample[1] = 1.0f - 2.0f*sample[1];
		if(sqrtf(sample[0]*sample[0] + sample[1]*sample[1]) <= 1.0f){
			found = true;
		}
	}
	return sample;
}
