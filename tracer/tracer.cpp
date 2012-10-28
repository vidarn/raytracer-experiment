#include "tracer.h"

void Tracer::render()
{
    int numPixels = m_viewPlane->getNumPixels();
    for(int i=0; i<numPixels; i++){
        Ray ray = m_viewPlane->getPixelRay(i);
        RGBA color = m_scene->trace(ray);
        m_viewPlane->setPixelValue(i,color);
    }
}
