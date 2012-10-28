#include "tracer.h"

void Tracer::render()
{
    int numPixels = m_viewPlane->getNumPixels();
    for(int i=0; i<numPixels; i++){
		int numSamples = m_sampler->getNumSamples();
		double invNumSamples = 1.0/(double)numSamples;
		RGBA color;
		color[3] = 1.0;
		for(int j=0; j<numSamples; j++){
			Point sample = m_sampler->getSquareSample();
			Ray ray = m_viewPlane->getPixelRay(i,sample);
			RGBA tmp = m_scene->trace(ray)*invNumSamples;
			color += tmp;
		}
        m_viewPlane->setPixelValue(i,color);
    }
}
