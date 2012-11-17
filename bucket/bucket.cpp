#include "bucket.h"

Bucket::Bucket(ViewPlane *viewPlane, Scene *scene, int startPixel[2], int endPixel[2], int width):m_viewPlane(viewPlane), m_scene(scene), m_width(width)
{
    for (int i = 0; i < 2; i++) {
        m_startPixel[i] = startPixel[i];
        m_endPixel[i] = endPixel[i];
    }
    m_sampler = new Sampler(2);
    m_done = false;
}

Bucket::~Bucket()
{
    delete m_sampler;
}

void Bucket::render()
{
    for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
        for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
            int i = x+y*m_width;
            int numSamples = m_sampler->getNumSamples();
            float invNumSamples = 1.0/(float)numSamples;
            RGBA color;
            for(int j=0; j<numSamples; j++){
                Point sample = m_sampler->getSquareSample();
                Ray ray = m_viewPlane->getPixelRay(i,sample);
                RGBA tmp = m_scene->trace(ray)*invNumSamples;
                color += tmp;
            }
            m_viewPlane->setPixelValue(i,color);
        }
    }
    m_done = true;
}
