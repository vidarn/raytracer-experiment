#ifndef __TRACER_H__
#define __TRACER_H__
#include "../scene/scene.h"
#include "../viewPlane/viewPlane.h"
#include "../sampler/sampler.h"
#include "../bucket/bucket.h"
#include <pthread.h>
#include <OSL/oslexec.h>
#include "../osl/simplerend.h"

class Tracer
{
	public:
        Tracer(Scene *scene, ViewPlane *viewPlane);
        ~Tracer();
        void render();

	private:
		Scene *m_scene;
		ViewPlane *m_viewPlane;
        Bucket **m_buckets;
};

#endif
