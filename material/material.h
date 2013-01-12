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
#include "../sampler/sampling.h"
#include "../file/image.h"

class Scene;

class Material
{
    public:
        Material();
        Material(std::ifstream &stream);
		void setColor(RGBA color);
		RGBA shade(ShadeRec shadeRec, Scene *scene, Sampling &sampling, Ray *nextRay, RGBA *reflectedMult, float *pdf);
        void shade(const Vec3 &in, const Vec3 &out, const ShadeRec &sr, RGBA* col, float pdf);
    private:
        RGBA m_color;
        float m_reflectivity;
        float m_glossiness;
        float m_ior;
		Image *m_texture;
        BRDF  *m_brdf;
};

#endif /* end of include guard: __MATERIAL_H__ */
