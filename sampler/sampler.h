#ifndef __SAMPLER_H__
#define __SAMPLER_H__
#include <vector>
#include "../utils/vec3.h"

class Sampler
{
	public:
		Sampler(int numSamples):m_numSamples(numSamples),m_currentSample(0){generateSamples();};
		~Sampler(){};
		void generateSamples();
		Vec3 getSquareSample();
		int getNumSamples(){return m_numSamples;};
	private:
		int m_currentSample;
		int m_numSamples;
		std::vector<Vec3> m_samples;
};

#endif
