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
    file.read(objects, m_lights,viewPlane,m_settings);
	std::vector<Triangle *> triangles;
	for(int i=0; i<objects.size();i++){
		objects[i]->refine(triangles);
	}
	std::cout << "num triangles: " << triangles.size() << std::endl;
	m_tree.build(triangles);
}

RGBA Scene::trace(Ray &ray, Sampling &sampling)
{
	RGBA col;
	if(ray.m_depth < 10){
		float minT = FLT_MAX;
		ShadeRec shadeRec;
		m_tree.hit(ray,shadeRec);
		if(shadeRec.m_hit)
		{
			shadeRec.m_hitPos = ray.getPointAtPos(shadeRec.m_hitT);
			shadeRec.m_triangle->shadeInfo(ray,shadeRec);
			shadeRec.setIncidentDirection(ray.m_dir.getNormalized());
			shadeRec.m_depth = ray.m_depth;
			if(shadeRec.getMaterial() != 0){
				col = shadeRec.getMaterial()->shade(shadeRec, this, sampling);
			}
			col[3] = 1.0f;
		}
        else{
            col[0] = col[1] = col[2] = 1.0f;
        }
	}
    return col;
}

float Scene::traceShadow(Ray &ray)
{
    float minT = FLT_MAX;
    ShadeRec shadeRec;
	m_tree.hit(ray,shadeRec);
	return !shadeRec.m_hit;
}

std::vector<GeometricObject *> Scene::getObjects()
{
    return m_objects;
}
