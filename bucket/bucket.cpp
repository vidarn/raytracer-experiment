#include "bucket.h"

Bucket::Bucket(ViewPlane *viewPlane, Scene *scene, int startPixel[2], int endPixel[2], int width):m_viewPlane(viewPlane), m_scene(scene), m_width(width)
{
    for (int i = 0; i < 2; i++) {
        m_startPixel[i] = startPixel[i];
        m_endPixel[i] = endPixel[i];
    }
    m_sampler = new Sampler(1);
    m_done = false;
    m_outlineSize = 5;
}

Bucket::~Bucket()
{
    delete m_sampler;
}

void Bucket::render()
{
    int pos[2];
    for(int corner = 0; corner < 4; corner++){
        for(int axis = 0; axis < 2; axis++){
            for(int a=0; a< m_outlineSize; a++){
                pos[axis] = a;
                pos[1-axis] = 0;
                if(corner < 2){
                    pos[0] += m_startPixel[0];
                }
                else{
                    pos[0] = m_endPixel[0] - pos[0]-1;
                }
                if(corner % 2){
                    pos[1] += m_startPixel[1];
                }
                else{
                    pos[1] = m_endPixel[1] - pos[1]-1;
                }
                RGBA pixel(1.0f,1.0f,1.0f,1.0f);
                int i = pos[0]+pos[1]*m_width;
                m_viewPlane->setPixelValue(i,pixel);
            }
        }
    }
    for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
        for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
            int i = x+y*m_width;
            int numSamples = m_sampler->getNumSamples();
            float invNumSamples = 1.0/(float)numSamples;
            RGBA color;
            for(int j=0; j<numSamples; j++){
                Vec3 sample = m_sampler->getSquareSample();
                Ray ray = m_viewPlane->getPixelRay(i,sample);
                RGBA tmp = m_scene->trace(ray)*invNumSamples;
                color += tmp;
            }
            m_pixels.push_back(color);
        }
    }
    int a = 0;
    for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
        for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
            int i = x+y*m_width;
            m_viewPlane->setPixelValue(i,m_pixels[a]);
            a++;
        }
    }
    m_done = true;
}
