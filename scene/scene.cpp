#include "scene.h"
#include "../geom/sphere.h"
#include "../geom/plane.h"
#include "../aggregates/aaBoundingBox.h"
#include "../utils/matrix4x4.h"
#include "../file/file.h"
#include <cstdlib>
#include <cfloat>

Scene::~Scene()
{
	// TODO: Free objects
}

void Scene::build(ViewPlane &viewPlane)
{
	std::vector<GeometricObject *> objects;
    File file("/tmp/test.scn");
    file.read(objects, m_lights,viewPlane,m_settings,&m_imageHandler);
	std::vector<Triangle *> triangles;
	for(int i=0; i<objects.size();i++){
		objects[i]->refine(triangles);
	}
	std::cout << "num triangles: " << triangles.size() << std::endl;
	m_triangles.build(triangles);
    m_numRays = 0;
}

void Scene::trace(Ray &ray, ShadeRec *shadeRec)
{
	if(ray.m_depth < 10){
		float minT = FLT_MAX;
		m_triangles.hit(ray,*shadeRec);
		if(shadeRec->m_hit)
		{
			shadeRec->m_hitPos = ray.getPointAtPos(shadeRec->m_hitT);
			shadeRec->m_triangle->shadeInfo(ray,*shadeRec);
			shadeRec->setIncidentDirection(ray.m_dir.getNormalized());
		}
	}
    shadeRec->m_depth = ray.m_depth;
    m_numRays++;
}

float Scene::traceShadow(Ray &ray, float maxT)
{
    float minT = FLT_MAX;
    ShadeRec shadeRec;
	m_triangles.hit(ray,shadeRec);
    m_numRays++;
	return !(shadeRec.m_hit && shadeRec.m_hitT < maxT);
}

std::vector<GeometricObject *> Scene::getObjects()
{
    return m_objects;
}
