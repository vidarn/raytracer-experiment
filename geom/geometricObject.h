#ifndef __GEOMETRIC_OBJECT_H__
#define __GEOMETRIC_OBJECT_H__
#include "../utils/ray.h"
#include "../utils/matrix4x4.h"
#include "../material/material.h"
#include "../shadeRec/shadeRec.h"
#include "../aggregates/aaBoundingBox.h"
#include "triangle.h"
#include <ostream>
#include <vector>

class BVH;

class GeometricObject
{
	public:
		Material *getMaterial(){return m_material;};
		Matrix4x4 getTransform(){return m_transform;};
		void setMaterial(Material *material){m_material = material;};
		void setTransform(Matrix4x4 transform){m_transform = transform;};
		virtual void refine(std::vector<Triangle *> &triangles) = 0;
		virtual AABoundingBox getBounds() = 0;
        virtual BVH *getBVH() = 0;
	protected:
		Material *m_material;
		Matrix4x4 m_transform;
        bool m_boundsCalculated;
        AABoundingBox m_bounds;
        bool m_bvhCalculated;
        BVH *m_bvh;
};

#endif
