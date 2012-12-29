#include "bucket.h"
#include "../sampler/sampling.h"

Bucket::Bucket(ViewPlane *viewPlane, Scene *scene):m_viewPlane(viewPlane), m_scene(scene)
{
    m_sampler = new RandomSampler();
}

Bucket::~Bucket()
{
    delete m_sampler;
}

void Bucket::render()
{
    while(true){
        sample();
    }
}

void Bucket::sample()
{
    Sampling sampling(1,m_scene->m_settings.m_lightSamples,3,1,1,m_sampler);
    Vec3 p = sampling.getSquareSample(0);
    int x = floor(p[0]*float(m_viewPlane->getWidth()));
    int y = floor(p[1]*float(m_viewPlane->getHeight()));
    Ray ray = m_viewPlane->getPixelRay(x,y,sampling);
    RGBA col = m_scene->trace(ray,sampling);
    m_viewPlane->setPixelValue(x,y,col);
}
