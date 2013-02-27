#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include <vector>
#include "../geom/geometricObject.h"
#include "aaBoundingBox.h"

class BVH;

class Instance
{
    public:
        Instance(int matId, int meshId, Matrix4x4 transform);
        void prepare(std::vector< GeometricObject* > &objects);
        void hit(Ray &ray, ShadeRec &sr) const;
        int  getNumTris();
        int  m_matId;
        int  m_meshId;
        AABoundingBox m_bounds;
        BVH *m_bvh;
        Matrix4x4 m_transform;
        Matrix4x4 m_normalinvTransform;
};

#endif /* end of include guard: __INSTANCE_H__ */
