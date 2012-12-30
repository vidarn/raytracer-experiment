#include "tracer.h"
#include <cmath>
#include <unistd.h>

static void *renderBucketProcess(void *threadid);

void Tracer::render()
{
	std::vector<pthread_t> threads;
	for(int i = 0;i < m_scene->m_settings.m_threads; i++){
		pthread_t thread;
		Bucket *bucket = new Bucket(m_viewPlane, m_scene,i);
		pthread_create(&thread, NULL, renderBucketProcess, (void*)(bucket));
		threads.push_back(thread);
	}
    while(true){
		sleep(2);
		m_viewPlane->saveToTiff();
    }
}

void *renderBucketProcess(void *param)
{
	Bucket *bucket = (Bucket*)param;
	bucket->render();
}

