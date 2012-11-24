#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "geometricObject.h"
#include "../aggregates/aaBoundingBox.h"
#include "../aggregates/kdTree.h"

class Mesh:public GeometricObject
{
    public:
		Mesh(){};
		Mesh(std::ifstream &stream, Matrix4x4 transform, Material *mat);
        void addPoint(Vec3 &point){m_points.push_back(point);};
        void addTriangle(Triangle &triangle){m_triangles.push_back(triangle);};
        void addNormal(Vec3 &normal){m_normals.push_back(normal);};
        Vec3 &getPoint(int id){return m_points[id];};
        Vec3 &getNormal(int id){return m_normals[id];};
        void calculateNormals();
        void addToNormal(int id, Vec3 normal);
		virtual void refine(std::vector<Triangle *> &triangles);
    private:
        std::vector<Vec3> m_points;
        std::vector<Vec3> m_normals;
        std::vector<Triangle> m_triangles;
};




#endif
