#include "scene.h"
#include "../geom/sphere.h"

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build()
{
    RGBA col = RGBA(1.0,0.0,0.0,1.0);
	Sphere *obj = new Sphere(0.9,col);
	m_objects.push_back(obj);
}

RGBA Scene::trace(Ray &ray)
{
    int numObjects = m_objects.size();
    RGBA col;
	ShadeRec sr = m_objects[0]->hit(ray);
	if(sr.getHit()){
		col = sr.getColor();
    }
    return col;
}
