#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include <fstream>
#include "../utils/rgba.h"
#include "../utils/ray.h"
#include "../shadeRec/shadeRec.h"
#include "../brdf/brdf.h"
#include "../brdf/lambert.h"
#include "../brdf/specularReflection.h"
#include "../brdf/phong.h"
#include "../brdf/torranceSparrow.h"
#include "../lights/light.h"
#include "../sampler/sampler.h"
#include <OSL/oslexec.h>
#include <OSL/oslclosure.h>

class Scene;
struct ClosureDesc;

class Material
{
    public:
        Material(OSL::ShadingSystem *shadingSys);
        ~Material();
        Material(std::ifstream &stream,OSL::ShadingSystem *shadingSys);
        void getClosure(const ShadeRec &sr, OSL::ShadingContext *shadingContext,
                RGBA *col, std::vector<ClosureDesc> &closureList);
		void shade(ShadeRec &shadeRec, Scene *scene, Sampler &sampler, Ray *nextRay,
                RGBA *reflectedMult, OSL::ShadingContext *shadingContext);
        void shade(const Vec3 &in, const Vec3 &out, const ShadeRec &sr, RGBA* col,
                OSL::ShadingContext *shadingContext);
    private:
        void processClosure(const OSL::ClosureColor *clos, OSL::Color3 col, std::vector<ClosureDesc> &closureList);
		OSL::ShadingAttribStateRef m_shader;
        OSL::ShadingSystem *m_shadingSystem;
};

struct ClosureDesc
{
    BRDF *m_brdf;
    OSL::Color3 m_col;
    const void *m_data;
    float m_weight;
    float m_accumWeight;
};
#endif /* end of include guard: __MATERIAL_H__ */
