#include "randomSampler.h"
#include <cstdlib>
#include <cmath>

RandomSampler::RandomSampler(long int seed)
{
	srand48_r(seed,&m_randData);
}

void RandomSampler::generate1DSequence(Vec3 *samples,int numSamples)
{
    for(int i=0;i<numSamples;i++){
        samples[i][0] = getRandomNumber();
    }
}

void RandomSampler::generateSquareSequence(Vec3 *samples,int numSamples)
{
    for(int i=0; i<numSamples; i++){
        for(int j=0; j<2; j++){
			samples[i][j] = getRandomNumber();
        }
    }
}

void RandomSampler::generateDiskSequence(Vec3 *samples,int numSamples)
{
    for(int i=0;i<numSamples;i++){
        bool found = false;
        while(!found){
            for(int j=0; j<2; j++){
				samples[i][j] = getRandomNumber();
            }
            samples[i][0] = 1.0f - 2.0f*samples[i][0];
            samples[i][1] = 1.0f - 2.0f*samples[i][1];
            if(sqrtf(samples[i][0]*samples[i][0] + samples[i][1]*samples[i][1]) <= 1.0f){
                found = true;
            }
        }
    }
}

void RandomSampler::generateHemisphereSequence(Vec3 *samples,int numSamples)
{
    generateSquareSequence(samples,numSamples);
}

float RandomSampler::getRandomNumber()
{
	double a = 1.0;
	while(float(a) == 1.0)
		drand48_r(&m_randData,&a);
	return float(a);
}
