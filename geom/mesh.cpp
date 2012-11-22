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
        Triangle triangle(this);
        triangle[0] = &m_points[pointIds[0]];
        triangle[1] = &m_points[pointIds[1]];
        triangle[2] = &m_points[pointIds[2]];
        triangle.setNormal(&m_normals[pointIds[0]],0);
        triangle.setNormal(&m_normals[pointIds[1]],1);
        triangle.setNormal(&m_normals[pointIds[2]],2);
		triangle.setMaterial(m_material);
        addTriangle(triangle);
    }
	applyTransformation();
    calculateNormals();
}

void Mesh::calculateNormals()
{
    for (int i = 0; i < m_triangles.size(); i++) {
        m_triangles[i].calculateNormal();
    }
    for (int i = 0; i < m_normals.size(); i++) {
        m_normals[i].normalize();
    }
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

void Mesh::refine(std::vector<Triangle *> &triangles)
{
	for (int i = 0; i < m_triangles.size(); i++) {
		triangles.push_back(&m_triangles[i]);
	}
}
