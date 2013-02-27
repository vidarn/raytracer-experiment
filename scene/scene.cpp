#include "scene.h"
#include "../geom/sphere.h"
#include "../geom/plane.h"
#include "../aggregates/aaBoundingBox.h"
#include "../utils/matrix4x4.h"
#include "../file/file.h"
#include <cstdlib>
#include <cfloat>
#include <OSL/genclosure.h>
#include <OSL/oslclosure.h>
#include <OSL/oslexec.h>

Scene::~Scene()
{
	// TODO: Free objects
	OSL::ShadingSystem::destroy(m_shadingSys);
}

void Scene::build(ViewPlane &viewPlane)
{
	m_shadingSys = OSL::ShadingSystem::create(&m_rend, 0, &m_errorhandler);
	registerClosures();

	std::vector<  GeometricObject* > objects;
	std::vector<  Instance*        > instances;
    File file("/tmp/test.scn");
    file.read(objects, instances, m_lights,viewPlane,m_settings,m_shadingSys);
	for(int i=0; i<instances.size();i++){
		instances[i]->prepare(objects);
	}
	std::vector<Triangle *> triangles;
	for(int i=0; i<objects.size();i++){
		objects[i]->refine(triangles);
	}
	std::cout << "num triangles: " << triangles.size() << std::endl;
	m_triangles.build(triangles, &instances);
    m_numRays = 0;
}

void Scene::registerClosures()
{
    // Describe the memory layout of each closure type to the OSL runtime
    enum { MaxParams = 32 };
    struct BuiltinClosures {
        const char* name;
        int id;
		OSL::ClosureParam params[MaxParams]; // upper bound
    };
    BuiltinClosures builtins[] = {
        { "diffuse"    , DIFFUSE_ID, {      CLOSURE_VECTOR_PARAM(DiffuseParams, N),
                                            CLOSURE_FINISH_PARAM(DiffuseParams) } },
        { "reflection"   , SPECULAR_ID,{    CLOSURE_VECTOR_PARAM(SpecularParams, N),
                                            CLOSURE_FLOAT_PARAM (SpecularParams, ior),
                                            CLOSURE_FINISH_PARAM(SpecularParams) } },
        { "phong"   , TORRANCE_SPARROW_ID,{ CLOSURE_VECTOR_PARAM(TorranceSparrowParams, N),
                                            CLOSURE_FLOAT_PARAM (TorranceSparrowParams, e),
                                            CLOSURE_FINISH_PARAM(TorranceSparrowParams) } },
        // mark end of the array
        { NULL, 0, {} }
    };

    for (int i = 0; builtins[i].name; i++) {
        m_shadingSys->register_closure(
            builtins[i].name,
            builtins[i].id,
            builtins[i].params,
            NULL, NULL, NULL);
    }
}


void Scene::trace(Ray &ray, ShadeRec *shadeRec)
{
	if(ray.m_depth < 10){
		float minT = FLT_MAX;
		m_triangles.hit(ray,*shadeRec);
		if(shadeRec->m_hit)
		{
            if(!shadeRec->m_calculated){
                shadeRec->m_hitPos = ray.getPointAtPos(shadeRec->m_hitT);
                shadeRec->m_triangle->shadeInfo(ray,*shadeRec);
            }
			shadeRec->setIncidentDirection(ray.m_dir.normalized());
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
