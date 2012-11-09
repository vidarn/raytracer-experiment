#ifndef __SHADE_REC_H__
#define __SHADE_REC_H__
#include "../utils/point.h"
#include "../utils/rgba.h"
#include "../utils/normal.h"

class Material;

class ShadeRec
{
    public:
        ShadeRec():m_hit(false),m_material(0){};
        ShadeRec(float hitT, Vec3 iDir, Normal normal, Material* material):m_hitT(hitT),m_iDir(iDir),m_normal(normal),m_hit(true),m_material(material){};
        void setHit(bool hit){m_hit = hit;};
        void setIncidentDirection(Vec3 dir){m_iDir = dir;};
        void setHitT(float t){m_hitT = t;};
        void setHitPos(Point pos){m_hitPos = pos;};
        void setNormal(Normal normal){m_normal = normal;};
        void setMaterial(Material *material){m_material = material;};
        float getHitT(){return m_hitT;};
        Point getHitPos(){return m_hitPos;};
        Vec3 getIncidentDirection(){return m_iDir;};
        bool getHit(){return m_hit;};
        Normal getNormal(){return m_normal;};
        Material *getMaterial(){return m_material;};
    private:
        bool m_hit;
        float m_hitT;
        Vec3 m_iDir;
		Normal m_normal;
		Point m_hitPos;
        Material *m_material;
};

#endif
