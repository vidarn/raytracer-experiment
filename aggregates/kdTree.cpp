#include "kdTree.h"
#include <cfloat>

void KDTree::build(std::vector<GeometricObject *> objects)
{
    for (int a = 0; a < 3; a++) {
        m_min[a] = FLT_MAX;
        m_max[a] = -FLT_MAX;
    }
    m_maxDepth = int(8+1.3f*log(objects.size()));
    m_maxPrims = 15;
    for(int i=0;i<objects.size();i++){
        m_objects.push_back(objects[i]);
    }
    float min[3], max[3];
    std::vector<AABoundingBox> bounds;
    for(int i =0; i<m_objects.size();i++){
        AABoundingBox b = AABoundingBox(m_objects[i]);
        b.getBounds(min,max);
        bounds.push_back(b);
        for (int a = 0; a < 3; a++) {
            m_min[a] = min[a] < m_min[a] ? min[a] : m_min[a];
            m_max[a] = max[a] > m_max[a] ? max[a] : m_max[a];
        }
    }
    m_bounds = new AABoundingBox(this);
    std::vector<int> overlappingObjects;
    for(int i=0; i<m_objects.size();i++){
        overlappingObjects.push_back(i);
    }
    buildNode(overlappingObjects,bounds,m_maxDepth,0);
}

void KDTree::buildNode(std::vector<int> &objects, std::vector<AABoundingBox> &bounds, int depth, char axis)
{
    if(objects.size() <= m_maxPrims || depth == 0){
        KDTreeLeafNode *leafNode = new KDTreeLeafNode(objects,this);
        m_nodes.push_back(leafNode);
    }
    else{
        // TODO: better calculation of split position
        float splitPosition;
        float min[3], max[3];
        bounds[objects[objects.size()/2]].getBounds(min,max);
        splitPosition = (min[axis] + max[axis])*0.5;
        KDTreeInteriorNode *interiorNode = new KDTreeInteriorNode(axis,splitPosition,this);
        interiorNode->setDepth(depth);
        m_nodes.push_back(interiorNode);
        std::vector<int> lowerObjects;
        std::vector<int> upperObjects;
        for (int i = 0; i < objects.size(); i++) {
            bounds[objects[i]].getBounds(min,max);
            if(min[axis] <= splitPosition){
                lowerObjects.push_back(objects[i]);
            }
            if(max[axis] >= splitPosition){
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

void KDTree::hit(Ray &ray, ShadeRec &sr) const
{
    float tmin, tmax;
    if(m_bounds->testBBox(ray,tmin,tmax)){
        m_nodes[0]->hit(ray,sr,tmin,tmax);
    }
}

void KDTree::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = m_min[i];
        max[i] = m_max[i];
    }
}

void KDTreeInteriorNode::hit(Ray &ray, ShadeRec &sr, float tMin, float tMax)
{
    if(ray.m_dir[m_axis] != 0.0f){
        KDTreeNode* nearNode = ray.m_origin[m_axis] < m_position ? getNodeBelow():getNodeAbove();
        KDTreeNode* farNode  = ray.m_origin[m_axis] < m_position ? getNodeAbove():getNodeBelow();
        float tHit = (m_position - ray.m_origin[m_axis])/ray.m_dir[m_axis];
        if(tHit > tMax){
            nearNode->hit(ray,sr,tMin,tMax);
        }
        else if(tHit < tMin){
            if(tHit > 0.0f){
                farNode->hit(ray,sr,tMin,tMax);
            }
            else if(tHit < 0.0f){
                nearNode->hit(ray,sr,tMin,tMax);
            }
            else{
                if(ray.m_dir[m_axis] < 0.0f){
                    farNode->hit(ray,sr,tMin,tMax);
                }
                else{
                    nearNode->hit(ray,sr,tMin,tMax);
                }
            }
        }
        else{
            if(tHit > 0.0f){
                nearNode->hit(ray,sr,tMin,tHit);
                if(!sr.getHit()){
                    farNode->hit(ray,sr,tHit,tMax);
                }
            }
            else{
                nearNode->hit(ray,sr,tMin,tMax);
            }
        }
    }
    else{
        if(ray.m_origin[m_axis] < m_position){
            getNodeBelow()->hit(ray,sr,tMin,tMax);
        }
        if(ray.m_origin[m_axis] > m_position){
            getNodeAbove()->hit(ray,sr,tMin,tMax);
        }
    }
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

KDTreeLeafNode::KDTreeLeafNode(std::vector<int> &objects, KDTree* owner)
{
    for(int i=0;i<objects.size();i++){
        m_objects.push_back(objects[i]);
    }
    m_owner = owner;
}

void KDTreeLeafNode::hit(Ray &ray, ShadeRec &sr, float tMin, float tMax)
{
    for(int i=0; i<m_objects.size();i++){
        ShadeRec tmp;
        m_owner->getObject(m_objects[i])->hit(ray, tmp);
        if(tmp.getHit()){
            if(tmp.getHitT() <= tMax && tmp.getHitT() >= tMin){
                tMax = tmp.getHitT();
                sr = tmp;
            }
        }
    }
}
