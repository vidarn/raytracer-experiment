#ifndef __SCENE_H__
#define __SCENE_H__
#include <vector>
#include "../utils/rgba.h"
#include "../utils/ray.h"
#include "../geom/geometricObject.h"
#include "../lights/directional.h"
#include "../lights/point.h"
#include "../lights/area.h"
#include "../aggregates/bvh.h"
#include "../aggregates/kdTree.h"
#include "../viewPlane/viewPlane.h"
#include "../settings/settings.h"
#include "../file/image.h"

class Scene
{
	public:
		Scene(){};
		~Scene();
		void build(ViewPlane &viewPlane);
        void trace(Ray &ray, ShadeRec *shadeRec);
        float traceShadow(Ray &ray, float maxT);
        std::vector<GeometricObject *> getObjects();
		std::vector<Light*> m_lights;
        Settings m_settings;
        int m_numRays;
	private:
		std::vector<GeometricObject *> m_objects;
		BVH m_triangles;
		RGBA m_bkgColor;
        ImageHandler m_imageHandler;
};

#endif
