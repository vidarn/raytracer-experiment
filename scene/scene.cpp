#include "scene.h"
#include "../geom/sphere.h"
#include "../utils/matrix4x4.h"

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build()
{
	Matrix4x4 transform;
	transform.setTranslation(0.0,0.5,0.0);
	transform.setScaling(1.0,1.0,1.0);
	transform = transform.invert();
    RGBA col = RGBA(1.0,0.0,0.0,1.0);
	Material mat = Material(col);
	Sphere *obj = new Sphere(0.9,mat,transform);
	m_objects.push_back(obj);
}

RGBA Scene::trace(Ray &ray)
{
    int numObjects = m_objects.size();
    RGBA col;
	Point tmpPoint = m_objects[0]->getTransform()*ray.m_origin;
	Vec3 tmpDir = m_objects[0]->getTransform()*ray.m_dir;
	Ray tmpRay(tmpPoint,tmpDir);
	ShadeRec sr = m_objects[0]->hit(tmpRay);
	if(sr.getHit()){
		col = m_objects[0]->getMaterial().shade(sr);
    }
    return col;
}
