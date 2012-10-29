#ifndef __BUCKET_H__
#define __BUCKET_H__
#include "../viewPlane/viewPlane.h"
#include "../scene/scene.h"
#include "../sampler/sampler.h"

class Bucket
{
	public:
		Bucket(ViewPlane *viewPlane, Scene *scene, Sampler *sampler, int startPixel, int endPixel):m_viewPlane(viewPlane), m_scene(scene), m_sampler(sampler), m_startPixel(startPixel), m_endPixel(endPixel){};
		void render();
	private:
		ViewPlane *m_viewPlane;
		Scene *m_scene;
		Sampler *m_sampler;
		int m_startPixel;
		int m_endPixel;
};

#endif
