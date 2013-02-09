#ifndef __BUCKET_H__
#define __BUCKET_H__
#include "../viewPlane/viewPlane.h"
#include "../scene/scene.h"
#include "../sampler/randomSampler.h"
#include "../sampler/stratifiedSampler.h"
#include <vector>
#include <OSL/oslexec.h>
#include <OSL/oslclosure.h>

class PathNode;

class Bucket
{
	public:
		Bucket(ViewPlane *viewPlane, Scene *scene, unsigned int id,
				OSL::ShadingSystem *shadingSys);
        ~Bucket();
		void render(int startX, int startY);
		int m_id;
		ViewPlane *m_viewPlane;
		Scene *m_scene;
	private:
		void getCol(ShadeRec &sr, RGBA &finalCol);
		void buildPath(PathNode* &path, Ray &ray, int &numNodes,
				const RGBA &startColor, int start, int end);
		void sample(int x, int y);
		Sampler *m_sampler;
        PathNode *m_cameraPath;
        PathNode *m_lightPath;
		OSL::ShadingSystem *m_shadingSystem;
		OSL::ShadingContext *m_shadingContext;
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
