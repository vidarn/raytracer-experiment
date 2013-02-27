#include "mesh.h"
#include <cfloat>
#include "../aggregates/bvh.h"


Mesh::Mesh(std::ifstream &stream, Matrix4x4 transform, Material *mat)
{
    m_boundsCalculated = false;
    m_bvhCalculated = false;
    m_transform = transform;
    m_material = mat;
    int num_verts;
    stream.read( reinterpret_cast<char*>( &num_verts ), sizeof num_verts );
    float pos[3];
    for(int i=0; i<num_verts; i++){
        for(int j=0; j<3; j++){
            stream.read( reinterpret_cast<char*>( &pos[j] ), sizeof pos[j] );
        }
        Vec3 point(pos[0],pos[1],pos[2]);
        m_transform.multVecMatrix(point,point);
        addPoint(point);
        Vec3 normal(0.0f,0.0f,0.0f);
        addNormal(normal);
    }
    int num_triangles;
    stream.read( reinterpret_cast<char*>( &num_triangles ), sizeof num_triangles );
	m_uvs.resize(num_triangles*3);
    for(int i=0; i<num_triangles; i++){
        Triangle triangle(this);
        int pointIds[3];
        for(int j=0; j<3; j++){
            stream.read( reinterpret_cast<char*>( &(pointIds[j]) ), sizeof pointIds[j] );
			triangle.setPoint(&m_points[pointIds[j]],j);
			triangle.setNormal(&m_normals[pointIds[j]],j);
        }
        for(int j=0; j<3; j++){
			Vec3 uv;
            stream.read( reinterpret_cast<char*>( &(uv[0]) ),      sizeof uv[0]);
            stream.read( reinterpret_cast<char*>( &(uv[1]) ),      sizeof uv[1]);
			m_uvs[i*3 + j] = uv;
			triangle.setUV(&(m_uvs[i*3 + j]),j);
		}
		triangle.setMaterial(m_material);
        addTriangle(triangle);
    }
    calculateNormals();
    for(int i=0; i<num_triangles; i++){
        m_triangles[i].computePlucker();
    }
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

void Mesh::refine(std::vector<Triangle *> &triangles)
{
	for (int i = 0; i < m_triangles.size(); i++) {
		triangles.push_back(&m_triangles[i]);
	}
}

AABoundingBox Mesh::getBounds()
{
    if(!m_boundsCalculated){
        float min[3], max[3];
        for (int i = 0; i < 3; i++) {
            min[i] = FLT_MAX;
            max[i] = -FLT_MAX;
        }
        for (int i = 0; i < m_triangles.size(); i++){
            m_triangles[i].addBounds(min,max);
        }
        m_bounds = AABoundingBox(min,max);
    }
    return m_bounds;
}

BVH *Mesh::getBVH()
{
    if(!m_bvhCalculated){
        std::vector<Triangle *> triangles;
        refine(triangles);
        m_bvh = new BVH;
        m_bvh->build(triangles, 0);
        m_bvhCalculated = true;
    }
    return m_bvh;
}
