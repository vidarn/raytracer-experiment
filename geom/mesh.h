#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "../utils/point.h"
#include "../utils/normal.h"
#include "geometricObject.h"

class Face;

class Mesh : public GeometricObject
{
    public:
		Mesh(){};
        void addPoint(Point &point){m_points.push_back(point);};
        void addFace(Face &face){m_faces.push_back(face);};
        void addNormal(Normal &normal){m_normals.push_back(normal);};
        Point &getPoint(int id){return m_points[id];};
        Normal &getNormal(int id){return m_normals[id];};
		virtual ShadeRec hit(Ray &ray) const;
        virtual void getBounds(double min[3], double max[3]) const;
        void calculateBounds();
        void calculateNormals();
        void addToNormal(int id, Normal normal);
    private:
        double m_min[3];
        double m_max[3];
        std::vector<Point> m_points;
        std::vector<Normal> m_normals;
        std::vector<Face> m_faces;
        Face* m_face;
};



class Face
{
    public:
        Face(Mesh *owner):m_owner(owner){};
        int &operator[](int id){return m_pointIds[id];};
        Point &getPoint(int id) const;
        Normal &getNormal(int id) const;
        void calculateNormal();
        void hit(Ray &ray, ShadeRec &sr) const;
    private:
        Normal m_normal;
        int m_pointIds[3];
        Mesh* m_owner;
};

#endif
