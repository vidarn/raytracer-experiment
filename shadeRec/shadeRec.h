#ifndef __SHADE_REC_H__
#define __SHADE_REC_H__
#include "../utils/point.h"
#include "../utils/rgba.h"
#include "../utils/normal.h"

class ShadeRec
{
    public:
        ShadeRec():m_hit(false){};
        ShadeRec(double hitT, Vec3 iDir, Normal normal):m_hitT(hitT),m_iDir(iDir),m_normal(normal),m_hit(true){};
        void setHit(bool hit){m_hit = hit;};
        void setIncidentDirection(Vec3 dir){m_iDir = dir;};
        void setHitT(double t){m_hitT = t;};
        void setHitPos(Point pos){m_hitPos = pos;};
        void setNormal(Normal normal){m_normal = normal;};
        double getHitT(){return m_hitT;};
        Point getHitPos(){return m_hitPos;};
        Vec3 getIncidentDirection(){return m_iDir;};
        bool getHit(){return m_hit;};
        Normal getNormal(){return m_normal;};
    private:
        bool m_hit;
        double m_hitT;
        Vec3 m_iDir;
		Normal m_normal;
		Point m_hitPos;
};

#endif
