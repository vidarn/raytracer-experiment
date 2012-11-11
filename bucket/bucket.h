#ifndef __BUCKET_H__
#define __BUCKET_H__
#include "../viewPlane/viewPlane.h"
#include "../scene/scene.h"
#include "../sampler/sampler.h"

class Bucket
{
	public:
		Bucket(ViewPlane *viewPlane, Scene *scene, int startPixel[2], int endPixel[2], int width);
        bool isDone(){return m_done;};
		void render();
	private:
		ViewPlane *m_viewPlane;
		Scene *m_scene;
		Sampler *m_sampler;
		int m_startPixel[2];
		int m_endPixel[2];
        int m_width;
        bool m_done;
};

#endif
