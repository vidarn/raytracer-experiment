#include "mesh.h"

ShadeRec Mesh::hit(Ray &ray) const
{
    ShadeRec sr;
    for (int i = 0; i < m_faces.size(); i++) {
        m_faces[i].hit(ray, sr);
    }
    if(sr.getHit()){
        sr.setMaterial(m_material);
    }
    return sr;
}

void Mesh::calculateNormals()
{
    for (int i = 0; i < m_faces.size(); i++) {
        m_faces[i].calculateNormal();
    }
    for (int i = 0; i < m_normals.size(); i++) {
        m_normals[i].normalize();
    }
}

void Mesh::addToNormal(int id, Normal normal)
{
    m_normals[id] += normal;
}

Point &Face::getPoint(int id) const
{
    return m_owner->getPoint(m_pointIds[id]);
}

Normal &Face::getNormal(int id) const
{
    return m_owner->getNormal(m_pointIds[id]);
}

void Face::calculateNormal()
{
    Vec3 v1 = getPoint(0) - getPoint(1);
    Vec3 v2 = getPoint(1) - getPoint(2);
    Vec3 v3 = v1.cross(v2);
    v3.normalize();
    m_normal = Normal(v3);
    for (int i = 0; i < 3; i++) {
        m_owner->addToNormal(m_pointIds[i],m_normal);
    }
}

void Face::hit(Ray &ray, ShadeRec &sr) const
{
    Point &p1 = getPoint(0);
    Point &p2 = getPoint(1);
    Point &p3 = getPoint(2);
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
        double b0 = (1.0 - b1 - b2);
        sr.setHit(true);
        sr.setHitT(t);
        Vec3 norVec;
        Vec3 tmpVec;
        tmpVec = getNormal(0).toVec3();
        tmpVec *= b0;
        norVec += tmpVec;
        tmpVec = getNormal(1).toVec3();
        tmpVec *= b1;
        norVec += tmpVec;
        tmpVec = getNormal(2).toVec3();
        tmpVec *= b2;
        norVec += tmpVec;
        norVec.normalize();
        sr.setNormal(norVec);
    }
}
