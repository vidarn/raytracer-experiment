#include "mesh.h"

ShadeRec Mesh::hit(Ray &ray) const
{
    ShadeRec sr;
    for (int i = 0; i < m_faces.size(); i++) {
        m_faces[i].hit(ray, sr);
    }
    return sr;
}

void Mesh::calculateNormals()
{
    for (int i = 0; i < m_faces.size(); i++) {
        m_faces[i].calculateNormal();
    }
}

Point &Face::getPoint(int id) const
{
    m_owner->getPoint(m_pointIds[id]);
}

void Face::calculateNormal()
{
    Vec3 v1 = getPoint(0) - getPoint(1);
    Vec3 v2 = getPoint(1) - getPoint(2);
    Vec3 v3 = v1.cross(v2);
    v3.normalize();
    m_normal = Normal(v3);
}

void Face::hit(Ray &ray, ShadeRec &sr) const
{
    Point &p1 = getPoint(0);
    Point &p2 = m_owner->getPoint(m_pointIds[1]);
    Point &p3 = m_owner->getPoint(m_pointIds[2]);
    Vec3 e1 = p2 - p1;
    Vec3 e2 = p3 - p1;
    Vec3 s1 = ray.m_dir.cross(e2);
    double divisor = s1.dot(e1);
    if(divisor == 0.0){
        return;
    }
    double invDivisor = 1.0/divisor;

    Vec3 d = ray.m_origin - p1;
    double b1 = d.dot(s1) * invDivisor;
    if(b1 < 0.0 || b1 > 1.0){
        return;
    }

    Vec3 s2 = d.cross(e1);
    double b2 = ray.m_dir.dot(s2) * invDivisor;
    if(b2 < 0.0 || b1 + b2 > 1.0){
        return;
    }

    double t = e2.dot(s2) * invDivisor;
    if(t < 0.0){
        return;
    }
    if(!sr.getHit() || t < sr.getHitT()){
        sr.setHit(true);
        sr.setHitT(t);
        sr.setNormal(m_normal);
    }
}
