#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "../utils/ray.h"
#include "../utils/vec3.h"
#include "../shadeRec/shadeRec.h"

class Mesh;

class Triangle
{
    public:
        Triangle(Mesh *owner):m_owner(owner){};
        Vec3 *&operator[](int id){return m_points[id];};
        void setNormal(Vec3 *normal,int i){m_normals[i] = normal;};
        void calculateNormal();
		void hit(Ray &ray, ShadeRec &sr) const;
        void getBounds(float min[3], float max[3]) const;
		void setMaterial(Material *material){m_material = material;};
		Material *getMaterial(){return m_material;};
    private:
        Vec3 *m_normals[3];
        Vec3 *m_points[3];
        Mesh* m_owner;
		Material *m_material;
};

#endif
