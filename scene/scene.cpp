#include "scene.h"
#include "../geom/plane.h"

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build()
{
	Plane *plane = new Plane;
	m_objects.push_back(plane);
}

RGBA Scene::trace(Ray &ray)
{
    int numObjects = m_objects.size();
    RGBA col;
    if(m_objects[0]->hit(ray)){
        col[0] = 1.0;
    }
	col[1] = ray.m_origin[1]*0.5+0.5;
	col[3] = 1.0;
    return col;
}
