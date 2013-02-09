#include "tracer.h"
#include <cmath>
#include <unistd.h>

static void *renderBucketProcess(void *threadid);

Tracer::Tracer(Scene *scene, ViewPlane *viewPlane)
	:m_scene(scene),m_viewPlane(viewPlane)
{
}

void Tracer::render()
{
	std::vector<pthread_t> threads;
	for(int i = 0;i < m_scene->m_settings.m_threads; i++){
		pthread_t thread;
		Bucket *bucket = new Bucket(m_viewPlane, m_scene,i,m_scene->m_shadingSys);
		pthread_create(&thread, NULL, renderBucketProcess, (void*)(bucket));
		threads.push_back(thread);
	}
    while(true){
		sleep(2);
		m_viewPlane->saveToImage();
        //std::cout << "num rays:    " << m_scene->m_numRays << std::endl;
    }
}
Tracer::~Tracer()
{
}

void *renderBucketProcess(void *param)
{
	Bucket *bucket = (Bucket*)param;
	int numBuckets = bucket->m_scene->m_settings.m_threads;
	float y = float(bucket->m_id)/float(numBuckets);
	y *= bucket->m_viewPlane->m_resolution[1];
	bucket->render(0,y);
}

