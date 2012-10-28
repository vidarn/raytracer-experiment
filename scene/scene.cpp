#include "scene.h"
#include "../geom/sphere.h"

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build()
{
	Sphere *obj = new Sphere(0.2);
	m_objects.push_back(obj);
}

RGBA Scene::trace(Ray &ray)
{
    int numObjects = m_objects.size();
    RGBA col;
    if(m_objects[0]->hit(ray)){
        col[0] = 1.0;
        col[1] = 1.0;
        col[2] = 1.0;
        col[3] = 1.0;
    }
    return col;
}
