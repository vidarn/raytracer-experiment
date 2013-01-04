#include "bucket.h"
#include "../sampler/sampling.h"

Bucket::Bucket(ViewPlane *viewPlane, Scene *scene, unsigned int id):m_viewPlane(viewPlane), m_scene(scene)
{
	m_id = id;
    m_sampler = new RandomSampler(134134+42343*id);
}

Bucket::~Bucket()
{
    delete m_sampler;
}

void Bucket::render()
{
	int numBuckets = m_scene->m_settings.m_threads;
	int bucketWidth = ceil(float(m_viewPlane->m_resolution[0])/float(numBuckets));
    while(true){
		for(int y=0; y<m_viewPlane->m_resolution[1]; y++){
			for(int x=m_id * bucketWidth; x<(m_id+1)*bucketWidth; x++){
				sample(x,y);
			}
		}
    }
}

void Bucket::sample(int x, int y)
{
	int lightSamples = m_scene->m_settings.m_lightSamples;
	int lightBounces = m_scene->m_settings.m_lightBounces;
    Sampling sampling(1,lightBounces*lightSamples,3,1,lightBounces,m_sampler);
    Vec3 p = sampling.getSquareSample(0);
    Ray ray = m_viewPlane->getPixelRay(x,y,sampling);
    RGBA col = m_scene->trace(ray,sampling);
    m_viewPlane->setPixelValue(x,y,col);
}
