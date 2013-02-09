#ifndef __BVH_H__
#define __BVH_H__
#include "../geom/triangle.h"
#include "../utils/ray.h"
#include "aaBoundingBox.h"
#include "../shadeRec/shadeRec.h"
#include <vector>
#include <stdint.h>

struct BVHTriangleInfo;
struct BVHBuildNode;
struct BVHLinearNode;

class BVH
{
	public:
		void build(std::vector<Triangle *> triangles);
		void hit(Ray &ray, ShadeRec &sr) const;
	private:
        int flattenTree(BVHBuildNode *buildNode, int *offset);
        void recursiveDelete(BVHBuildNode *node);
        BVHBuildNode *recursiveBuild(int start, int end, int *numTotalNodes);
        void findSAHSplit(int dim, int start, int end, AABoundingBox &bboxCentroids, AABoundingBox &bbox, float *splitCost, int *splitBucket);
		void createLeafNode(int start, int end, AABoundingBox &bbox, BVHBuildNode *node);
		std::vector<Triangle *> m_triangles;
		std::vector<Triangle *> m_orderedTriangles;
		std::vector<BVHTriangleInfo> m_buildData;
        BVHLinearNode *m_root;
        void writeObjFile(const char *filename);
        void recursiveWriteObjFile(std::vector<Vec3> &points, int offset );
};

struct BVHTriangleInfo
{
	BVHTriangleInfo(int id, const AABoundingBox &bbox):
		m_triangleId(id), m_bbox(bbox) {
			m_centroid = bbox.min() * 0.5f;
			m_centroid += bbox.max() * 0.5f;
		}
	int m_triangleId;
	Vec3 m_centroid;
	AABoundingBox m_bbox;
};

struct BVHLinearNode
{
    AABoundingBox m_bbox;
    union {
        uint32_t m_firstTriOffset;
        uint32_t m_secondChildOffset;
    };
    uint8_t m_numTris;
    uint8_t m_axis;
};

struct BVHBuildNode
{
	BVHBuildNode() { m_children[0] = m_children[1] = NULL; }
	void initInterior(int axis, BVHBuildNode *child0, BVHBuildNode *child1){
		m_children[0] = child0; m_children[1] = child1;
		AABoundingBox tmpBBox = child0->m_bbox;
		tmpBBox.join(child1->m_bbox);
		m_bbox = tmpBBox;
		m_splitAxis = axis;
		m_numTris = 0;
	}
	void initLeaf(int firstTriangle, int numTriangles, const AABoundingBox &bbox){
		m_firstTriOffset = firstTriangle;
		m_numTris = numTriangles;
		m_bbox = bbox;
	}
	AABoundingBox m_bbox;
	BVHBuildNode *m_children[2];
	int m_splitAxis, m_firstTriOffset, m_numTris;
};

class CompareToMid
{
	public:
		CompareToMid(int dim,float mid):m_dim(dim),m_mid(mid){};
		bool operator()(const BVHTriangleInfo &a){return a.m_centroid[m_dim] < m_mid;}
	private:
		int m_dim;
		float m_mid;
};

class CompareToBucket
{
	public:
		CompareToBucket(int dim,float bucket, int numBuckets, const AABoundingBox &bound):m_dim(dim),m_bucket(bucket), m_numBuckets(numBuckets),m_centroidBounds(bound){};
		bool operator()(const BVHTriangleInfo &a) const;
	private:
		int m_dim;
        int m_bucket;
        int m_numBuckets;
        const AABoundingBox &m_centroidBounds;
};

class SAHBucket
{
    public:
        SAHBucket(){m_numTris =0;}
        int m_numTris;
        AABoundingBox m_bbox;
};

#endif /* end of include guard: __BVH_H__ */
