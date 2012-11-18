#include "uniformSampler.h"

UniformSampler::UniformSampler(int numSubdivisions):
m_numSubdivisions(numSubdivisions),
m_numSamples(numSubdivisions*numSubdivisions),
m_currentSample(0)
{}

Vec3 UniformSampler::getSquareSample()
{
    Vec3 sample;
	int x = m_currentSample / m_numSubdivisions;
	int y = m_currentSample % m_numSubdivisions;
	sample[0] = float(x)/float(m_numSubdivisions+2);
	sample[1] = float(y)/float(m_numSubdivisions+2);
	m_currentSample = (m_currentSample+1)%m_numSamples;
    return sample;
}
