#ifndef __SHADE_REC_H__
#define __SHADE_REC_H__
#include "../utils/rgba.h"
#include "../utils/vec3.h"
#include "../brdf/brdf.h"
#include <cfloat>

class Material;
class Triangle;
class BRDF;

class ShadeRec
{
    public:
        ShadeRec():m_hit(false),m_material(0),m_hitT(FLT_MAX),m_calculated(false){};
        ShadeRec(float hitT, Vec3 iDir, Vec3 normal, Material* material):m_hitT(hitT),m_iDir(iDir),m_normal(normal),m_hit(true),m_material(material),m_calculated(false){};
        void setIncidentDirection(Vec3 dir){m_iDir = dir;};
        void setNormal(Vec3 normal){m_normal = normal;};
        void setMaterial(Material *material){m_material = material;};
        Vec3 getIncidentDirection(){return m_iDir;};
        Vec3 getNormal(){return m_normal;};
        Material *getMaterial(){return m_material;};
        Material *m_material;
		const Triangle *m_triangle;
        float m_hitT;
		Vec3  m_hitPos;
        bool  m_hit;
		int   m_depth;
		Vec3  m_normal;
		Vec3  m_uv;
        Vec3  m_iDir;
        bool  m_calculated;
		BRDF *m_brdf;
};

#endif
