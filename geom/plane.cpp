#include "plane.h"

ShadeRec Plane::hit(Ray &ray) const
{
	ShadeRec tmp;
    double origin_z = ray.m_origin[2];
    double direction_z = ray.m_dir[2];
    if(direction_z == 0.0){
        return tmp;
    }
    double t = origin_z/direction_z;
    if(t>0.0){
        tmp.setHit(true);
        tmp.setHitT(t);
        Normal normal(0.0,0.0,-1.0);
        tmp.setNormal(normal);
    }
    return tmp;
}
