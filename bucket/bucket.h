#ifndef __BUCKET_H__
#define __BUCKET_H__
#include "../viewPlane/viewPlane.h"
#include "../scene/scene.h"
#include "../sampler/randomSampler.h"
#include "../sampler/stratifiedSampler.h"
#include <vector>

class Bucket
{
	public:
		Bucket(ViewPlane *viewPlane, Scene *scene, unsigned int id);
        ~Bucket();
		void render(int startX, int startY);
		int m_id;
		ViewPlane *m_viewPlane;
		Scene *m_scene;
	private:
		void sample(int x, int y);
		Sampler *m_sampler;
};

#endif
