#ifndef __SHADE_REC_H__
#define __SHADE_REC_H__
#include "../utils/point.h"
#include "../utils/rgba.h"
#include "../utils/normal.h"

class ShadeRec
{
    public:
        ShadeRec():m_hit(false){m_color[3] = 0.0;};
        ShadeRec(double hitT, Vec3 iDir, RGBA color, Normal normal):m_hitT(hitT),m_iDir(iDir),m_color(color),m_normal(normal),m_hit(true){};
        void setHit(bool hit){m_hit = hit;};
        void setIncidentDirection(Vec3 dir){m_iDir = dir;};
        void setHitT(double t){m_hitT = t;};
        void setColor(RGBA color){m_color = color;};
        void setNormal(Normal normal){m_normal = normal;};
        double getHitT(){return m_hitT;};
        Vec3 getIncidentDirection(){return m_iDir;};
        bool getHit(){return m_hit;};
        RGBA getColor(){return m_color;};
        Normal getNormal(){return m_normal;};
    private:
        bool m_hit;
        double m_hitT;
        Vec3 m_iDir;
        RGBA m_color;
		Normal m_normal;
};

#endif
