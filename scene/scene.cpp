#include "scene.h"
#include "../geom/sphere.h"
#include "../geom/plane.h"
#include "../aggregates/aaBoundingBox.h"
#include "../utils/matrix4x4.h"
#include "../file/file.h"
#include <cfloat>

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build()
{
	std::vector<GeometricObject *> objects;
    File file("/tmp/test.scn");
    file.read(objects, m_lights);
	std::vector<Triangle *> triangles;
	for(int i=0; i<objects.size();i++){
		objects[i]->refine(triangles);
	}
	std::cout << "num triangles: " << triangles.size() << std::endl;
	m_tree.build(triangles);
}

RGBA Scene::trace(Ray &ray)
{
	RGBA col;
    float minT = FLT_MAX;
    ShadeRec shadeRec;
	m_tree.hit(ray,shadeRec);
    if(shadeRec.getHit())
    {
        Vec3 hitPos = ray.getPointAtPos(shadeRec.getHitT());
        shadeRec.setHitPos(hitPos);
        shadeRec.setIncidentDirection(ray.m_dir.getNormalized());
        for(int i = 0; i < m_lights.size(); i++){
            if(shadeRec.getMaterial() != 0){
                col = shadeRec.getMaterial()->shade(shadeRec, m_lights[i], this) + col;
            }
        }
        col[3] = 1.0f;
    }
    return col;
}

float Scene::traceShadow(Ray &ray)
{
    float minT = FLT_MAX;
    ShadeRec shadeRec;
	m_tree.hit(ray,shadeRec);
    if(shadeRec.getHit()){
        return 0.0f;
    }
    else{
        return 1.0f;
    }
}

std::vector<GeometricObject *> Scene::getObjects()
{
    return m_objects;
}
