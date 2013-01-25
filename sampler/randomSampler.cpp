#include "randomSampler.h"
#include <cstdlib>
#include <cmath>

RandomSampler::RandomSampler(long int seed)
{
	srand48_r(seed,&m_randData);
}

float RandomSampler::getSample()
{
	double a = 1.0;
	while(float(a) == 1.0)
		drand48_r(&m_randData,&a);
	return float(a);
}
