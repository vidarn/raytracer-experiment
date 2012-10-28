#include "sampler.h"

void Sampler::generateSamples()
{
	int n = m_numSamples;
	m_numSamples *= m_numSamples;
	double xd, yd;
	double invN = 1.0/((double)(n+1));
	yd = invN;
	for(int y = 0; y<n; y++){
		xd = invN;
		for(int x = 0; x<n; x++){
			Point tmpPoint(xd,yd,0.0);
			m_samples.push_back(tmpPoint);
			xd += invN;
		}
		yd += invN;
	}
}

Point Sampler::getSquareSample()
{
	int tmp = m_currentSample;
	m_currentSample = (m_currentSample + 1) % m_numSamples;
	return m_samples[tmp];
}
