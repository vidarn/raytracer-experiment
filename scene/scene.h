#ifndef __SCENE_H__
#define __SCENE_H__
#include <vector>
#include "../utils/rgba.h"
#include "../utils/ray.h"
#include "../geom/geometricObject.h"
#include "../lights/directional.h"
#include "../lights/point.h"
#include "../lights/area.h"
#include "../aggregates/kdTree.h"
#include "../viewPlane/viewPlane.h"
#include "../settings/settings.h"

class Scene
{
	public:
		Scene(){};
		~Scene();
		void build(ViewPlane &viewPlane);
        RGBA trace(Ray &ray, Sampling &sampling);
        float traceShadow(Ray &ray);
        std::vector<GeometricObject *> getObjects();
		std::vector<Light*> m_lights;
        Settings m_settings;
	private:
		std::vector<GeometricObject *> m_objects;
		KDTree m_tree;
		RGBA m_bkgColor;
};

#endif
