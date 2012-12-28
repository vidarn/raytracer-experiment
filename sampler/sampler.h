#ifndef __SAMPLER_H__
#define __SAMPLER_H__
#include <vector>
#include "../utils/vec3.h"

class Sampler
{
	public:
		virtual void generate1DSequence(Vec3 *samples,int numSamples) = 0;
		virtual void generateSquareSequence(Vec3 *samples,int numSamples) = 0;
		virtual void generateDiskSequence(Vec3 *samples,int numSamples) = 0;
		virtual void generateHemisphereSequence(Vec3 *samples,int numSamples) = 0;
};

#endif
