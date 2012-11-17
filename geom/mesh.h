#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "geometricObject.h"
#include "../aggregates/aaBoundingBox.h"
#include "../aggregates/collection.h"
#include "../aggregates/kdTree.h"

class Face;

class Mesh : public GeometricObject
{
    public:
		Mesh(){};
		Mesh(std::ifstream &stream, Matrix4x4 transform, Material *mat);
        void addPoint(Vec3 &point){m_points.push_back(point);};
        void addFace(Face &face){m_faces.push_back(face);};
        void addNormal(Vec3 &normal){m_normals.push_back(normal);};
        Vec3 &getPoint(int id){return m_points[id];};
        Vec3 &getNormal(int id){return m_normals[id];};
		virtual void hit(Ray &ray, ShadeRec &sr) const;
        virtual void getBounds(float min[3], float max[3]) const;
        void calculateBounds();
        void calculateNormals();
        void populateCollection();
        void addToNormal(int id, Vec3 normal);
    private:
        float m_min[3];
        float m_max[3];
        std::vector<Vec3> m_points;
        std::vector<Vec3> m_normals;
        std::vector<Face> m_faces;
        KDTree m_kdTree;
        Face* m_face;
};



class Face : public GeometricObject
{
    public:
        Face(Mesh *owner):m_owner(owner){};
        int &operator[](int id){return m_pointIds[id];};
        Vec3 &getPoint(int id) const;
        Vec3 &getNormal(int id) const;
        void calculateNormal();
		virtual void hit(Ray &ray, ShadeRec &sr) const;
        virtual void getBounds(float min[3], float max[3]) const;
    private:
        Vec3 m_normal;
        int m_pointIds[3];
        Mesh* m_owner;
};

#endif
