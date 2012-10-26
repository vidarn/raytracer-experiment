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
};
