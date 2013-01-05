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

void Bucket::render(int startX, int startY)
{
	int x = startX;
	int y = startY;
    while(true){
		while(y<m_viewPlane->m_resolution[1]){
			while(x<m_viewPlane->m_resolution[0]){
				sample(x,y);
				x++;
			}
			x = 0;
			y++;
		}
		y = 0;
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
