#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "../utils/rgba.h"
#include "../shadeRec/shadeRec.h"
#include "../brdf/brdf.h"
#include "../brdf/lambert.h"
#include "../brdf/phong.h"

class Material
{
    public:
        Material(){};
        Material(RGBA color):m_color(color){};
		void setColor(RGBA color);
		RGBA shade(ShadeRec shadeRec);
    private:
        BRDF *m_brdf;
        RGBA m_color;
};

#endif /* end of include guard: __MATERIAL_H__ */
