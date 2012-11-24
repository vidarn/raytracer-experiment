#ifndef __BUCKET_H__
#define __BUCKET_H__
#include "../viewPlane/viewPlane.h"
#include "../scene/scene.h"
#include "../sampler/randomSampler.h"
#include "../sampler/uniformSampler.h"
#include <vector>

class Bucket
{
	public:
		Bucket(ViewPlane *viewPlane, Scene *scene, int startPixel[2], int endPixel[2], int width);
        ~Bucket();
        bool isDone(){return m_done;};
        bool borderDrawn(){return m_borderDrawn;};
		void render();
	private:
		void drawBorder();
		void sample(int numSamples, int pixelId, std::vector<RGBA> &samples);
		float getVariance(std::vector<RGBA *> &samples);
		ViewPlane *m_viewPlane;
		Scene *m_scene;
		Sampler *m_sampler;
		int m_startPixel[2];
		int m_endPixel[2];
        int m_width;
		int m_numPixels;
		int m_numMinSamples;
		int m_numMaxSamples;
        bool m_done;
        bool m_borderDrawn;
        int m_outlineSize;
};

#endif
