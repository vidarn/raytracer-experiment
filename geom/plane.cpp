#include "plane.h"

ShadeRec Plane::hit(Ray &ray) const
{
	ShadeRec sr;
    double origin_z = ray.m_origin[2];
    double direction_z = ray.m_dir[2];
    if(direction_z == 0.0){
        return sr;
    }
    double t = -origin_z/direction_z;
    if(t>0.0){
        sr.setHit(true);
        sr.setHitT(t);
        Normal normal(0.0,0.0,-1.0);
        sr.setNormal(normal);
        sr.setMaterial(m_material);
    }
    return sr;
}
