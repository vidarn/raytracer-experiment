#ifndef __SCENE_H__
#define __SCENE_H__
#include <vector>
#include "../utils/rgba.h"
#include "../utils/ray.h"
#include "../geom/geometricObject.h"
#include "../lights/directional.h"
#include "../lights/point.h"
#include "../aggregates/kdTree.h"
#include "../viewPlane/viewPlane.h"

class Scene
{
	public:
		Scene(){};
		~Scene();
		void build(ViewPlane &viewPlane);
        RGBA trace(Ray &ray);
        float traceShadow(Ray &ray);
        std::vector<GeometricObject *> getObjects();
	private:
		std::vector<GeometricObject *> m_objects;
		KDTree m_tree;
		std::vector<Light*> m_lights;
		RGBA m_bkgColor;
};

#endif
