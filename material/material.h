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
#include "../file/image.h"

class Scene;
class MaterialLayer;

class Material
{
    public:
        Material();
        ~Material();
        Material(std::ifstream &stream, ImageHandler *imageHandler);
		void setColor(RGBA color);
        RGBA getColor(const ShadeRec &sr);
		void shade(ShadeRec &shadeRec, Scene *scene, Sampler &sampler, Ray *nextRay, RGBA *reflectedMult, float *pdf);
        void shade(const Vec3 &in, const Vec3 &out, const ShadeRec &sr, RGBA* col, float pdf);
    private:
		void absorb(RGBA *col, MaterialLayer &layer, const float cosTheta);
        RGBA m_color;
        RGBA m_coatingColor;
        float m_reflectivity;
        float m_glossiness;
        float m_ior;
		float m_thickness;
        MaterialLayer *m_layers;
        int m_numLayers;
		RGBA *m_layerReflectionBuffers;
		Vec3 *m_outDirs;
		RGBA *m_outColors;
		float *m_layerWeights;
        ImageHandler* m_imageHandler;
        int m_image;
};

class MaterialLayer
{
    public:
        BRDF *m_brdf;
        float m_thickness;
        float m_ior;
        RGBA m_absorbColor;
};

#endif /* end of include guard: __MATERIAL_H__ */
