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
        Tracer(Scene *scene, ViewPlane *viewPlane, Sampler *sampler):m_scene(scene),m_viewPlane(viewPlane),m_sampler(sampler){};
        ~Tracer(){};
        void render();

	private:
		Scene *m_scene;
		ViewPlane *m_viewPlane;
		Sampler *m_sampler;
};

#endif
