#include "mesh.h"
#include <cfloat>


Mesh::Mesh(std::ifstream &stream, Matrix4x4 transform, Material *mat)
{
    m_transform = transform;
    m_material = mat;
    int num_verts;
    stream.read( reinterpret_cast<char*>( &num_verts ), sizeof num_verts );
    std::cout << "numVerts: " << num_verts << std::endl;
    float pos[3];
    for(int i=0; i<num_verts; i++){
        for(int j=0; j<3; j++){
            stream.read( reinterpret_cast<char*>( &pos[j] ), sizeof pos[j] );
        }
        Point point(pos);
        addPoint(point);
        Normal normal;
        addNormal(normal);
    }
    int num_triangles;
    stream.read( reinterpret_cast<char*>( &num_triangles ), sizeof num_triangles );
    std::cout << "numTriangles: " << num_triangles << std::endl;
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
    Point &p1 = getPoint(0);
    Point &p2 = getPoint(1);
    Point &p3 = getPoint(2);
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
