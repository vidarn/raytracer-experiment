#ifndef __TRACER_H__
#define __TRACER_H__
#include "../scene/scene.h"
#include "../viewPlane/viewPlane.h"
#include "../sampler/sampler.h"
#include "../bucket/bucket.h"
#include <pthread.h>

class Tracer
{
	public:
        Tracer(Scene *scene, ViewPlane *viewPlane):m_scene(scene),m_viewPlane(viewPlane){};
        ~Tracer(){};
        void render();

	private:
		Scene *m_scene;
		ViewPlane *m_viewPlane;
        Bucket **m_buckets;
};

#endif
