#include "kdTree.h"

void KDTree::build(std::vector<GeometricObject *> objects)
{
    m_maxDepth = 4;
    m_maxPrims = 15;
    for(int i=0;i<objects.size();i++){
        m_objects.push_back(objects[i]);
    }
    std::vector<AABoundingBox> bounds;
    for(int i =0; i<m_objects.size();i++){
        AABoundingBox b = AABoundingBox(m_objects[i]);
        bounds.push_back(b);
    }
    std::vector<int> overlappingObjects;
    for(int i=0; i<m_objects.size();i++){
        overlappingObjects.push_back(i);
    }
    buildNode(overlappingObjects,bounds,m_maxDepth,1);
}

void KDTree::buildNode(std::vector<int> objects, std::vector<AABoundingBox> bounds, int depth, char axis)
{
    if(objects.size() <= m_maxPrims || depth == 0){
        KDTreeLeafNode *leafNode = new KDTreeLeafNode(objects,this);
        m_nodes.push_back(leafNode);
    }
    else{
        // TODO: better calculation of split position
        float splitPosition;
        float min[3], max[3];
        for(int i=0;i<objects.size();i++){
            bounds[objects[i]].getBounds(min,max);
            splitPosition += (min[axis] + max[axis])*0.5;
        }
        splitPosition /= float(objects.size());
        KDTreeInteriorNode *interiorNode = new KDTreeInteriorNode(axis,splitPosition,this);
        m_nodes.push_back(interiorNode);
        std::vector<int> lowerObjects;
        std::vector<int> upperObjects;
        for (int i = 0; i < objects.size(); i++) {
            bounds[objects[i]].getBounds(min,max);
            if(min[axis] < splitPosition){
                lowerObjects.push_back(objects[i]);
            }
            if(max[axis] > splitPosition){
                upperObjects.push_back(objects[i]);
            }
        }
        char nextAxis = (axis+1)%3;
        int nextDepth = depth-1;
        interiorNode->setLowerChildIndex(m_nodes.size());
        buildNode(lowerObjects,bounds,nextDepth,nextAxis);
        interiorNode->setUpperChildIndex(m_nodes.size());
        buildNode(upperObjects,bounds,nextDepth,nextAxis);
    }
}

ShadeRec KDTree::hit(Ray &ray) const
{
	ShadeRec sr;
    sr = m_nodes[0]->hit(ray);
	return sr;
}

void KDTree::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = -1;
        max[i] = 1;
    }
}

ShadeRec KDTreeInteriorNode::hit(Ray &ray)
{
    ShadeRec low, high;
    low = getNodeBelow()->hit(ray);
    high = getNodeAbove()->hit(ray);
    if(high.getHit()){
        if(!low.getHit() || low.getHitT() > high.getHitT()){
            low = high;
        }
    }
	return low;
}

KDTreeNode *KDTreeInteriorNode::getNodeBelow()
{
    KDTreeNode *ret;
    ret = m_owner->getNode(m_nodeBelowIndex);
    return ret;
}

KDTreeNode *KDTreeInteriorNode::getNodeAbove()
{
    KDTreeNode *ret;
    ret = m_owner->getNode(m_nodeAboveIndex);
    return ret;
}

KDTreeLeafNode::KDTreeLeafNode(std::vector<int> objects, KDTree* owner)
{
    for(int i=0;i<objects.size();i++){
        m_objects.push_back(objects[i]);
    }
    m_owner = owner;
}

ShadeRec KDTreeLeafNode::hit(Ray &ray)
{
    ShadeRec sr;
    for(int i=0; i<m_objects.size();i++){
        ShadeRec tmp;
        tmp = m_owner->getObject(m_objects[i])->hit(ray);
        if(tmp.getHit()){
            if(!(sr.getHit()) || tmp.getHitT() < sr.getHitT()){
                sr = tmp;
            }
        }
    }
	return sr;
}
