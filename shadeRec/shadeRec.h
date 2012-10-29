#ifndef __SHADE_REC_H__
#define __SHADE_REC_H__
#include "../utils/point.h"
#include "../utils/rgba.h"

class ShadeRec
{
    public:
        ShadeRec():m_hit(false){m_color[3] = 0.0;};
        ShadeRec(Point hitPos, Vec3 iDir, RGBA color):m_hitPos(hitPos),m_iDir(iDir),m_color(color),m_hit(true){};
        void setHit(bool hit){m_hit = hit;};
        void setIncidentDirection(Vec3 dir){m_iDir = dir;};
        void setHitPos(Point pos){m_hitPos = pos;};
        void setColor(RGBA color){m_color = color;};
        Point getHitPos(){return m_hitPos;};
        Vec3 getIncidentDirection(){return m_iDir;};
        bool getHit(){return m_hit;};
        RGBA getColor(){return m_color;};
    private:
        bool m_hit;
        Point m_hitPos;
        Vec3 m_iDir;
        RGBA m_color;
};

#endif
