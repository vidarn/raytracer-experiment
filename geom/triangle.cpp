#include "triangle.h"
#include "mesh.h"
#include <cfloat>

Vec3 &Triangle::getPoint(int id) const
{
    return m_owner->getPoint(m_pointIds[id]);
}

Vec3 &Triangle::getNormal(int id) const
{
    return m_owner->getNormal(m_pointIds[id]);
}

void Triangle::calculateNormal()
{
    Vec3 v1 = getPoint(0) - getPoint(1);
    Vec3 v2 = getPoint(1) - getPoint(2);
    m_normal = v1.cross(v2);
    m_normal.normalize();
    for (int i = 0; i < 3; i++) {
        m_owner->addToNormal(m_pointIds[i],m_normal);
    }
}

void Triangle::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = FLT_MAX;
        max[i] = -FLT_MAX;
        for (int j = 0; j < 3; j++){
            float val = getPoint(j)[i];
            if(val < min[i]){
                min[i] = val;
            }
            if(val > max[i]){
                max[i] = val;
            }
        }
    }
}

void Triangle::hit(Ray &ray, ShadeRec &sr) const
{
    Vec3 &p1 = getPoint(0);
    Vec3 &p2 = getPoint(1);
    Vec3 &p3 = getPoint(2);
    Vec3 e1 = p2 - p1;
    Vec3 e2 = p3 - p1;
    Vec3 s1 = ray.m_dir.cross(e2);
    float divisor = s1.dot(e1);
    if(divisor == 0.0){
        return;
    }
    float invDivisor = 1.0/divisor;

    Vec3 d = ray.m_origin - p1;
    float b1 = d.dot(s1) * invDivisor;
    if(b1 < 0.0 || b1 > 1.0){
        return;
    }

    Vec3 s2 = d.cross(e1);
    float b2 = ray.m_dir.dot(s2) * invDivisor;
    if(b2 < 0.0 || b1 + b2 > 1.0){
        return;
    }

    float t = e2.dot(s2) * invDivisor;
    if(t < 0.0){
        return;
    }
    float b0 = (1.0 - b1 - b2);
    sr.setHit(true);
    sr.setHitT(t);
    Vec3 norVec;
    Vec3 tmpVec;
    tmpVec = getNormal(0);
    tmpVec *= b0;
    norVec += tmpVec;
    tmpVec = getNormal(1);
    tmpVec *= b1;
    norVec += tmpVec;
    tmpVec = getNormal(2);
    tmpVec *= b2;
    norVec += tmpVec;
    norVec.normalize();
    sr.setNormal(norVec);
	sr.setMaterial(m_material);
}
