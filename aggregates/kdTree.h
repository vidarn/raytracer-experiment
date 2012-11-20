#ifndef __KD_TREE_H_
#define __KD_TREE_H_
#include "../geom/geometricObject.h"
#include "aaBoundingBox.h"
#include <vector>

class KDTreeNode;
class BoundEdge;

class KDTree: public GeometricObject
{
    public:
        virtual void getBounds(float min[3], float max[3]) const;
		virtual void hit(Ray &ray, ShadeRec &sr) const;
        void build(std::vector<GeometricObject *> objects);
        GeometricObject* getObject(int i){return m_objects[i];};
        KDTreeNode* getNode(int i){return m_nodes[i];};
    private:
        void buildNode(std::vector<int> &objects, std::vector<AABoundingBox> &bounds, int depth, char axis, AABoundingBox totalBounds);
        int findSplitPos(std::vector<int> &objects, std::vector<AABoundingBox> &bounds, AABoundingBox &totalBounds, char axis);
        char findSplitAxis(std::vector<int> &objects, std::vector<AABoundingBox> &bounds, AABoundingBox &totalBounds);

        BoundEdge *m_boundEdges[3];
        AABoundingBox *m_bounds;
        float m_min[3], m_max[3];
        std::vector<KDTreeNode *> m_nodes;
        std::vector<GeometricObject *> m_objects;
        int m_maxDepth;
        int m_maxPrims;
        static const float m_emptyBonus = 0.0f;
        static const float m_traversalCost = 1.0f;
        static const float m_intersectCost = 80.0f;
};

class KDTreeNode
{
    public:
        virtual bool isInterior() const = 0;
		virtual void hit(Ray &ray, ShadeRec &sr, float tMin, float tMax) = 0;
    protected:
        KDTree *m_owner;
};

class KDTreeInteriorNode: public KDTreeNode
{
    public:
        KDTreeInteriorNode(char axis, float position, KDTree* owner):m_axis(axis),m_position(position){m_owner = owner;};
        KDTreeInteriorNode(KDTree* owner){m_owner = owner;};
        virtual bool isInterior() const { return true;};
		virtual void hit(Ray &ray, ShadeRec &sr, float tMin, float tMax);
        KDTreeNode *getNodeBelow();
        KDTreeNode *getNodeAbove();
        void setLowerChildIndex(int i){m_nodeBelowIndex = i;};
        void setUpperChildIndex(int i){m_nodeAboveIndex = i;};
        void setDepth(int depth){m_depth = depth;};
        void setAxis(char axis){m_axis = axis;};
        void setPosition(float pos){m_position = pos;};
    private:
        char m_axis;
        int m_depth;
        float m_position;
        int m_nodeAboveIndex;
        int m_nodeBelowIndex;
};

class KDTreeLeafNode: public KDTreeNode
{
    public:
        KDTreeLeafNode(std::vector<int> &objects, KDTree* owner);
        virtual bool isInterior() const { return false;};
		virtual void hit(Ray &ray, ShadeRec &sr, float tMin, float tMax);
    private:
        std::vector<int> m_objects;
};

class BoundEdge
{
    public:
        BoundEdge();
        bool operator<(const BoundEdge &other) const;
        float m_pos;
        int m_primNum;
        bool m_start;
};

#endif
