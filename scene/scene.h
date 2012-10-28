#ifndef __SCENE_H__
#define __SCENE_H__
#include <vector>
#include "../utils/rgba.h"
#include "../utils/ray.h"
#include "../geom/geometricObject.h"

class Scene
{
	public:
		Scene(){};
		~Scene();
		void build();
        RGBA trace(Ray &ray);
	private:
		std::vector<GeometricObject*> m_objects;
		RGBA m_bkgColor;
};

#endif
