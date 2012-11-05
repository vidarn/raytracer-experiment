#include "plane.h"
#include <cfloat>

void Plane::getBounds(double min[3], double max[3]) const
{
    min[0] = -0.1;
    max[0] = 0.1;
    min[1] = -0.1;
    max[1] = 0.1;
    min[2] = DBL_MIN;
    max[2] = DBL_MAX;
}

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
