#ifndef __SHADE_REC_H__
#define __SHADE_REC_H__
#include "../utils/rgba.h"
#include "../utils/vec3.h"

class Material;

class ShadeRec
{
    public:
        ShadeRec():m_hit(false),m_material(0){};
        ShadeRec(float hitT, Vec3 iDir, Vec3 normal, Material* material):m_hitT(hitT),m_iDir(iDir),m_normal(normal),m_hit(true),m_material(material){};
        void setHit(bool hit){m_hit = hit;};
        void setIncidentDirection(Vec3 dir){m_iDir = dir;};
        void setHitT(float t){m_hitT = t;};
        void setHitPos(Vec3 pos){m_hitPos = pos;};
        void setNormal(Vec3 normal){m_normal = normal;};
        void setMaterial(Material *material){m_material = material;};
        float getHitT(){return m_hitT;};
        Vec3 getHitPos(){return m_hitPos;};
        Vec3 getIncidentDirection(){return m_iDir;};
        bool getHit(){return m_hit;};
        Vec3 getNormal(){return m_normal;};
        Material *getMaterial(){return m_material;};
    private:
        bool m_hit;
        float m_hitT;
        Vec3 m_iDir;
		Vec3 m_normal;
		Vec3 m_hitPos;
        Material *m_material;
};

#endif
