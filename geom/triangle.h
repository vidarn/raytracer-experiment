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
        int &operator[](int id){return m_pointIds[id];};
        Vec3 &getPoint(int id) const;
        Vec3 &getNormal(int id) const;
        void calculateNormal();
		void hit(Ray &ray, ShadeRec &sr) const;
        void getBounds(float min[3], float max[3]) const;
		void setMaterial(Material *material){m_material = material;};
		Material *getMaterial(){return m_material;};
    private:
        Vec3 m_normal;
        int m_pointIds[3];
        Mesh* m_owner;
		Material *m_material;
};

#endif
