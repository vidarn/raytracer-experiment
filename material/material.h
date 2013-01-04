#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include <fstream>
#include "../utils/rgba.h"
#include "../shadeRec/shadeRec.h"
#include "../brdf/brdf.h"
#include "../brdf/lambert.h"
#include "../brdf/specularReflection.h"
#include "../brdf/phong.h"
#include "../lights/light.h"
#include "../sampler/sampling.h"

class Scene;

class Material
{
    public:
        Material():m_color(RGBA(0.7,0.7,0.7,1.0)),m_reflectivity(0.0){};
        Material(std::ifstream &stream);
		void setColor(RGBA color);
		RGBA shade(ShadeRec shadeRec, Scene *scene, Sampling &sampling);
    private:
        RGBA m_color;
        float m_reflectivity;
        float m_glossiness;
};

#endif /* end of include guard: __MATERIAL_H__ */
