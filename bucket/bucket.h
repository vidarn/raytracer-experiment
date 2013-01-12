#ifndef __BUCKET_H__
#define __BUCKET_H__
#include "../viewPlane/viewPlane.h"
#include "../scene/scene.h"
#include "../sampler/randomSampler.h"
#include "../sampler/stratifiedSampler.h"
#include <vector>

class PathNode;

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
		void buildPath(PathNode* &path, Ray &ray, Sampling &sampling, int &numNodes,const RGBA &startColor, int start, int end, int samplingId);
		Sampler *m_sampler;
        PathNode *m_cameraPath;
        PathNode *m_lightPath;
};

class PathNode
{
	public:
		PathNode(){};
		ShadeRec m_sr;
		RGBA m_accumColor;
		Vec3 m_incident;
};

#endif
