#include "tracer.h"

static void *renderBucketProcess(void *threadid);

void Tracer::render()
{
	std::vector<pthread_t> threads;
	std::vector<Bucket> buckets;
    int numPixels = m_viewPlane->getNumPixels();
	int numThreads = 6;
	int pixelsPerThreads = numPixels/numThreads;
	for(int i = 0;i < numThreads; i++){
		int startPixel = i*pixelsPerThreads;
		int endPixel = (i+1)*pixelsPerThreads;
		if(i == numThreads-1){
			endPixel = numPixels;
		}
		Bucket bucket(m_viewPlane, m_scene, m_sampler, startPixel, endPixel);
		buckets.push_back(bucket);
		pthread_t thread;
		threads.push_back(thread);
	}
	for(int i = 0;i < numThreads; i++){
		pthread_create(&(threads[i]), NULL, renderBucketProcess, (void*)&(buckets[i]));
	}
	for(int i = 0;i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}
}

void *renderBucketProcess(void *param)
{
	Bucket *bucket = (Bucket*)param;
	bucket->render();
}

