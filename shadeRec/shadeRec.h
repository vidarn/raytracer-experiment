#ifndef __SHADE_REC_H__
#define __SHADE_REC_H__
#include "../utils/rgba.h"
#include "../utils/vec3.h"
#include <cfloat>

class Material;
class Triangle;

class ShadeRec
{
    public:
        ShadeRec():m_hit(false),m_material(0),m_hitT(FLT_MAX){};
        ShadeRec(float hitT, Vec3 iDir, Vec3 normal, Material* material):m_hitT(hitT),m_iDir(iDir),m_normal(normal),m_hit(true),m_material(material){};
        void setIncidentDirection(Vec3 dir){m_iDir = dir;};
        void setNormal(Vec3 normal){m_normal = normal;};
        void setMaterial(Material *material){m_material = material;};
        Vec3 getIncidentDirection(){return m_iDir;};
        Vec3 getNormal(){return m_normal;};
        Material *getMaterial(){return m_material;};
        float m_hitT;
		Vec3 m_hitPos;
		const Triangle *m_triangle;
        bool m_hit;
		int m_depth;
    private:
        Vec3 m_iDir;
		Vec3 m_normal;
        Material *m_material;
};

#endif
