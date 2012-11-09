#include "mesh.h"
#include <cfloat>

ShadeRec Mesh::hit(Ray &ray) const
{
    ShadeRec sr = m_kdTree.hit(ray);
    sr.setMaterial(m_material);
    return sr;
}

void Mesh::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = m_min[i];
        max[i] = m_max[i];
    }
}

void Mesh::calculateBounds()
{
    for (int i = 0; i < 3; i++) {
        m_min[i] = DBL_MAX;
        m_max[i] = DBL_MIN;
    }
    for (int i = 0; i < m_points.size(); i++) {
        for (int j = 0; j < 3; j++) {
            if( m_points[i][j] < m_min[j]){
                m_min[j] = m_points[i][j];
            }
            if( m_points[i][j] > m_max[j]){
                m_max[j] = m_points[i][j];
            }
        }
    }
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

void Mesh::populateCollection()
{
	std::vector<GeometricObject *> objects;
	for (int i = 0; i < m_faces.size(); i++) {
		objects.push_back(&(m_faces[i]));
	}
	m_kdTree.build(objects);
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

void Face::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = DBL_MAX;
        max[i] = DBL_MIN;
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

ShadeRec Face::hit(Ray &ray) const
{
    ShadeRec sr;
    Point &p1 = getPoint(0);
    Point &p2 = getPoint(1);
    Point &p3 = getPoint(2);
    Vec3 e1 = p2 - p1;
    Vec3 e2 = p3 - p1;
    Vec3 s1 = ray.m_dir.cross(e2);
    float divisor = s1.dot(e1);
    if(divisor == 0.0){
        return sr;
    }
    float invDivisor = 1.0/divisor;

    Vec3 d = ray.m_origin - p1;
    float b1 = d.dot(s1) * invDivisor;
    if(b1 < 0.0 || b1 > 1.0){
        return sr;
    }

    Vec3 s2 = d.cross(e1);
    float b2 = ray.m_dir.dot(s2) * invDivisor;
    if(b2 < 0.0 || b1 + b2 > 1.0){
        return sr;
    }

    float t = e2.dot(s2) * invDivisor;
    if(t < 0.0){
        return sr;
    }
    if(!sr.getHit() || t < sr.getHitT()){
        float b0 = (1.0 - b1 - b2);
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
    return sr;
}
