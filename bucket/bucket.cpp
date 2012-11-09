#include "bucket.h"

void Bucket::render()
{
    for(int i=m_startPixel; i<m_endPixel; i++){
		int numSamples = m_sampler->getNumSamples();
		float invNumSamples = 1.0/(float)numSamples;
		RGBA color;
		for(int j=0; j<numSamples; j++){
			Point sample = m_sampler->getSquareSample();
			Ray ray = m_viewPlane->getPixelRay(i,sample);
			RGBA tmp = m_scene->trace(ray)*invNumSamples;
			color += tmp;
		}
		color[3] = 1.0;
        m_viewPlane->setPixelValue(i,color);
    }
}
