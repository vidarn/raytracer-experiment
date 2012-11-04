#include "scene.h"
#include "../geom/sphere.h"
#include "../geom/plane.h"
#include "../utils/matrix4x4.h"
#include <cfloat>

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build()
{
	Matrix4x4 transform, tmpMat, worldTransform;
	tmpMat.setTranslation(0.0,0.0,-50.0);
    worldTransform = worldTransform*tmpMat;

	tmpMat.setTranslation(-0.5,0.0,1.0);
    transform = transform*tmpMat;
	transform = transform.invert();
    transform = worldTransform*transform;
    RGBA col = RGBA(0.8,0.2,0.1,1.0);
	Material mat = Material(col);
	Sphere *obj = new Sphere(0.9,mat,transform);
	m_objects.push_back(obj);

    transform.setIdentity();
	tmpMat.setScaling(0.5,0.5,0.5);
    transform = transform*tmpMat;
	tmpMat.setTranslation(0.5,0.0,0.0);
    transform = transform*tmpMat;
	transform = transform.invert();
    transform = worldTransform*transform;
    col = RGBA(0.1,0.2,0.8,1.0);
	mat = Material(col);
	obj = new Sphere(0.9,mat,transform);
	m_objects.push_back(obj);

    transform.setIdentity();
	tmpMat.setTranslation(0.0,0.0,10.0);
    transform = transform*tmpMat;
	transform = transform.invert();
    transform = worldTransform*transform;
    col = RGBA(0.2,0.2,0.2,1.0);
	mat = Material(col);
	Plane *plan = new Plane(mat,transform);
	m_objects.push_back(plan);

	transform.setIdentity();
	tmpMat.setTranslation(0.6,2.0,-2.0);
    transform = transform*tmpMat;
    transform = worldTransform*transform;
	PointLight *directionalLight = new PointLight(1.0,transform);
	m_lights.push_back(directionalLight);
}

RGBA Scene::trace(Ray &ray)
{
	RGBA col;
    int numObjects = m_objects.size();
    double minT = DBL_MAX;
	for (int i = 0; i < numObjects; i++) {
		Point tmpPoint = m_objects[i]->getTransform()*ray.m_origin;
		Vec3 tmpDir = m_objects[i]->getTransform()*ray.m_dir;
		Ray tmpRay(tmpPoint,tmpDir);

		ShadeRec sr = m_objects[i]->hit(tmpRay);
		if(sr.getHit()){
            if(sr.getHitT() < minT){
                minT = sr.getHitT();
                Point hitPos = ray.getPointAtPos(sr.getHitT());
                sr.setHitPos(hitPos);
                sr.setIncidentDirection(ray.m_dir.getNormalized());
                col = m_objects[i]->getMaterial().shade(sr, m_lights[0]);
            }
		}
	}
    return col;
}
