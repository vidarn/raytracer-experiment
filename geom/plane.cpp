#include "plane.h"
#include <cfloat>

void Plane::getBounds(float min[3], float max[3]) const
{
    min[0] = -0.1;
    max[0] = 0.1;
    min[1] = -0.1;
    max[1] = 0.1;
    min[2] = -FLT_MAX;
    max[2] = FLT_MAX;
}

void Plane::hit(Ray &ray, ShadeRec &sr) const
{
    float origin_z = ray.m_origin[2];
    float direction_z = ray.m_dir[2];
    if(direction_z == 0.0){
        return;
    }
    float t = -origin_z/direction_z;
    if(t>0.0){
        sr.setHit(true);
        sr.setHitT(t);
        Normal normal(0.0,0.0,-1.0);
        sr.setNormal(normal);
        sr.setMaterial(m_material);
    }
}
