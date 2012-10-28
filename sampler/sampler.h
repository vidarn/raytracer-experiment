#ifndef __SAMPLER_H__
#define __SAMPLER_H__
#include "../utils/point.h"
#include <vector>

class Sampler
{
	public:
		Sampler(int numSamples):m_numSamples(numSamples),m_currentSample(0){generateSamples();};
		~Sampler(){};
		void generateSamples();
		Point getSquareSample();
		int getNumSamples(){return m_numSamples;};
	private:
		int m_currentSample;
		int m_numSamples;
		std::vector<Point> m_samples;
};

#endif
