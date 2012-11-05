#include "scene.h"
#include "../geom/sphere.h"
#include "../geom/plane.h"
#include "../geom/meshReaders/objReader.h"
#include "../aggregates/aaBoundingBox.h"
#include "../aggregates/collection.h"
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
	tmpMat.setRotation(1,-1.4);
    //worldTransform = worldTransform*tmpMat;

    Collection *collection = new Collection();
    collection->setTransform(worldTransform);
	m_objects.push_back(collection);


	tmpMat.setTranslation(-0.5,0.0,1.0);
    transform = transform*tmpMat;
	transform = transform.invert();
    RGBA col = RGBA(0.8,0.2,0.1,1.0);
	Material *mat = new Material(col);
	Sphere *obj = new Sphere(0.9,mat,transform);
    collection->addObject(obj);

    transform.setIdentity();
	tmpMat.setScaling(0.5,0.5,0.5);
    transform = transform*tmpMat;
	tmpMat.setTranslation(0.5,0.0,0.0);
    transform = transform*tmpMat;
	transform = transform.invert();
    col = RGBA(0.1,0.2,0.8,1.0);
	mat = new Material(col);
	obj = new Sphere(0.9,mat,transform);
    collection->addObject(obj);

    transform.setIdentity();
	tmpMat.setTranslation(0.0,0.0,10.0);
    transform = transform*tmpMat;
	transform = transform.invert();
    col = RGBA(0.8,0.2,0.2,1.0);
	mat = new Material(col);
	Plane *plan = new Plane(mat,transform);
    collection->addObject(plan);

    transform.setIdentity();
	tmpMat.setScaling(0.4,0.4,0.4);
    transform = transform*tmpMat;
	tmpMat.setTranslation(0.0,0.0,-1.0);
    transform = transform*tmpMat;
	transform = transform.invert();
    col = RGBA(0.2,0.8,0.8,1.0);
	mat = new Material(col);
    ObjReader objReader;
	Mesh *mesh = objReader.read("monkey.obj");
    mesh->setTransform(transform);
    mesh->setMaterial(mat);
    AABoundingBox *bbox = new AABoundingBox(mesh);
    collection->addObject(bbox);

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
    ShadeRec shadeRec;
	for (int i = 0; i < numObjects; i++) {
		Point tmpPoint = m_objects[i]->getTransform()*ray.m_origin;
		Vec3 tmpDir = m_objects[i]->getTransform()*ray.m_dir;
		Ray tmpRay(tmpPoint,tmpDir);

		ShadeRec sr = m_objects[i]->hit(tmpRay);
		if(sr.getHit()){
            if(sr.getHitT() < minT){
                minT = sr.getHitT();
                shadeRec = sr;
            }
		}
	}
    if(shadeRec.getHit())
    {
        Point hitPos = ray.getPointAtPos(shadeRec.getHitT());
        shadeRec.setHitPos(hitPos);
        shadeRec.setIncidentDirection(ray.m_dir.getNormalized());
        if(shadeRec.getMaterial() != 0){
            col = shadeRec.getMaterial()->shade(shadeRec, m_lights[0]);
        }
    }
    return col;
}
