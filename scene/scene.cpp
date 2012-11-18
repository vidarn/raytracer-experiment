#include "scene.h"
#include "../geom/sphere.h"
#include "../geom/plane.h"
#include "../geom/meshReaders/objReader.h"
#include "../aggregates/aaBoundingBox.h"
#include "../aggregates/collection.h"
#include "../utils/matrix4x4.h"
#include "../file/file.h"
#include <cfloat>

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build()
{
	/*Matrix4x4 transform, tmpMat, worldTransform;
	tmpMat.setTranslation(0.0,0.0,-50.0);
    worldTransform = worldTransform*tmpMat;
	tmpMat.setRotation(1,-0.4);
    worldTransform = worldTransform*tmpMat;*/

    File file("/tmp/test.scn");
    file.read(m_objects, m_lights);

	/*transform.setIdentity();
	tmpMat.setTranslation(0.6,2.0,-2.0);
    transform = transform*tmpMat;
    transform = worldTransform*transform;
	PointLight *directionalLight = new PointLight(1.0,transform);
	m_lights.push_back(directionalLight);*/
}

RGBA Scene::trace(Ray &ray)
{
	RGBA col;
    int numObjects = m_objects.size();
    float minT = FLT_MAX;
    ShadeRec shadeRec;
	for (int i = 0; i < numObjects; i++) {
		ShadeRec sr;
        m_objects[i]->hit(ray,sr);
		if(sr.getHit()){
            if(sr.getHitT() < minT){
                minT = sr.getHitT();
                shadeRec = sr;
            }
		}
	}
    if(shadeRec.getHit())
    {
        Vec3 hitPos = ray.getPointAtPos(shadeRec.getHitT());
        shadeRec.setHitPos(hitPos);
        shadeRec.setIncidentDirection(ray.m_dir.getNormalized());
        for(int i = 0; i < m_lights.size(); i++){
            if(shadeRec.getMaterial() != 0){
                col = shadeRec.getMaterial()->shade(shadeRec, m_lights[i]) + col;
            }
        }
        col[3] = 1.0f;
    }
    return col;
}

std::vector<GeometricObject *> Scene::getObjects()
{
    std::vector<GeometricObject *> objects;
    for(int i=0;i<m_objects.size();i++){
        objects.push_back(m_objects[i]);
    }
    return objects;
}
