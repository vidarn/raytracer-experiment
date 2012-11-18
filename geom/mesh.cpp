#include "mesh.h"
#include <cfloat>


Mesh::Mesh(std::ifstream &stream, Matrix4x4 transform, Material *mat)
{
    m_transform = transform;
    m_material = mat;
    int num_verts;
    stream.read( reinterpret_cast<char*>( &num_verts ), sizeof num_verts );
    float pos[3];
    for(int i=0; i<num_verts; i++){
        for(int j=0; j<3; j++){
            stream.read( reinterpret_cast<char*>( &pos[j] ), sizeof pos[j] );
        }
        Vec3 point(pos);
        addPoint(point);
        Vec3 normal;
        addNormal(normal);
    }
    int num_triangles;
    stream.read( reinterpret_cast<char*>( &num_triangles ), sizeof num_triangles );
    for(int i=0; i<num_triangles; i++){
        int pointIds[3];
        for(int j=0; j<3; j++){
            stream.read( reinterpret_cast<char*>( &(pointIds[j]) ), sizeof pointIds[j] );
        }
        Face face(this);
        face[0] = pointIds[0];
        face[1] = pointIds[1];
        face[2] = pointIds[2];
        addFace(face);
    }
	applyTransformation();
    calculateNormals();
    calculateBounds();
    populateCollection();
}

void Mesh::hit(Ray &ray, ShadeRec &sr) const
{
    m_kdTree.hit(ray, sr);
    sr.setMaterial(m_material);
}

void Mesh::getBounds(float min[3], float max[3]) const
{
    m_kdTree.getBounds(min,max);
}

void Mesh::calculateBounds()
{
    for (int i = 0; i < 3; i++) {
        m_min[i] = FLT_MAX;
        m_max[i] = -FLT_MAX;
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

void Mesh::addToNormal(int id, Vec3 normal)
{
    m_normals[id] += normal;
}

void Mesh::applyTransformation()
{
	for (int i = 0; i < m_points.size(); i++) {
		m_points[i] = m_transform.multPoint(m_points[i]);
	}
}

Vec3 &Face::getPoint(int id) const
{
    return m_owner->getPoint(m_pointIds[id]);
}

Vec3 &Face::getNormal(int id) const
{
    return m_owner->getNormal(m_pointIds[id]);
}

void Face::calculateNormal()
{
    Vec3 v1 = getPoint(0) - getPoint(1);
    Vec3 v2 = getPoint(1) - getPoint(2);
    m_normal = v1.cross(v2);
    m_normal.normalize();
    for (int i = 0; i < 3; i++) {
        m_owner->addToNormal(m_pointIds[i],m_normal);
    }
}

void Face::getBounds(float min[3], float max[3]) const
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

void Face::hit(Ray &ray, ShadeRec &sr) const
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
}
