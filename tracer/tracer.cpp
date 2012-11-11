#include "tracer.h"
#include <cmath>
#include <unistd.h>

static void *renderBucketProcess(void *threadid);

void Tracer::render()
{
	std::vector<Bucket *> buckets;
    int numPixels = m_viewPlane->getNumPixels();
    int numBuckets[2] = {5,5};
    int numTotalBuckets = numBuckets[0] * numBuckets[1];
	int pixelsPerBucket[2];
    pixelsPerBucket[0] = ceil(float(m_viewPlane->getWidth())/float(numBuckets[0]));
    pixelsPerBucket[1] = ceil(float(m_viewPlane->getHeight())/float(numBuckets[1]));
    int startPixel[2];
    int endPixel[2];
	for(int y = 0;y < numBuckets[1]; y++){
        startPixel[1] = pixelsPerBucket[1]*y;
        endPixel[1] = pixelsPerBucket[1]*(y+1);
        endPixel[1] = endPixel[1] < m_viewPlane->getHeight() ? endPixel[1] : m_viewPlane->getHeight();
        for(int x = 0;x < numBuckets[0]; x++){
            startPixel[0] = pixelsPerBucket[0]*x;
            endPixel[0] = pixelsPerBucket[0]*(x+1);
            endPixel[0] = endPixel[0] < m_viewPlane->getWidth() ? endPixel[0] : m_viewPlane->getWidth();
            Bucket *bucket = new Bucket(m_viewPlane, m_scene, startPixel, endPixel,m_viewPlane->getWidth());
            buckets.push_back(bucket);
        }
	}
	std::vector<pthread_t> threads;
	int numThreads = 6;
	for(int i = 0;i < numThreads; i++){
		pthread_t thread;
		threads.push_back(thread);
    }
    int bucketMap[numThreads];
    int currentBucket = 0;
	for(int i = 0;i < numThreads; i++){
		pthread_create(&(threads[i]), NULL, renderBucketProcess, (void*)(buckets[currentBucket]));
        bucketMap[i] = currentBucket;
        currentBucket++;
	}
    int numDoneBuckets = 0;
    while(numDoneBuckets < numTotalBuckets){
        for(int i = 0;i < numThreads; i++){
            if(bucketMap[i] != -1){
                if(buckets[bucketMap[i]]->isDone()){
                    pthread_join(threads[i],NULL);
                    numDoneBuckets++;
                    std::cout << "bucket " << bucketMap[i] << " done!\n";
                    bucketMap[i] = -1;
                    if(currentBucket < numTotalBuckets){
                        pthread_create(&(threads[i]), NULL, renderBucketProcess, (void*)(buckets[currentBucket]));
                        bucketMap[i] = currentBucket;
                        currentBucket++;
                    }
                }
            }
        }
        usleep(10);
    }
    for(int i=0; i<numTotalBuckets; i++){
        delete buckets[i];
    }
}

void *renderBucketProcess(void *param)
{
	Bucket *bucket = (Bucket*)param;
	bucket->render();
}

