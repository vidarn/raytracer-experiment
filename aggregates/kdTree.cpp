#include "kdTree.h"
#include <cfloat>
#include <algorithm>
#include <cstdlib>

void KDTree::build(std::vector<Triangle *> objects)
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
        AABoundingBox b;
        b.extend(m_objects[i]);
        b.getBounds(min,max);
        bounds.push_back(b);
        for (int a = 0; a < 3; a++) {
            m_min[a] = min[a] < m_min[a] ? min[a] : m_min[a];
            m_max[a] = max[a] > m_max[a] ? max[a] : m_max[a];
        }
    }
    m_bounds = new AABoundingBox();
    m_bounds->setMin(m_min);
    m_bounds->setMax(m_max);
    std::vector<int> overlappingObjects;
    for(int i=0; i<m_objects.size();i++){
        overlappingObjects.push_back(i);
    }
    for(int i=0; i<3; i++){
        m_boundEdges[i] = new BoundEdge[2*m_objects.size()];
    }
    std::cout << "started building tree!\n";
    buildNode(overlappingObjects,bounds,m_maxDepth,*m_bounds);
    std::cout << "tree built!\n";
}

void KDTree::buildNode(std::vector<int> &objects, std::vector<AABoundingBox> &bounds, int depth,  AABoundingBox totalBounds)
{
    if(objects.size() <= m_maxPrims || depth == 0){
        KDTreeLeafNode *leafNode = new KDTreeLeafNode(objects,this);
        m_nodes.push_back(leafNode);
    }
    else{
        int splitPosition;
        char axis = findSplitAxis(objects,bounds,totalBounds);
        KDTreeInteriorNode *interiorNode = new KDTreeInteriorNode(this);
        int numRetries = 0;
        while(numRetries < 3){
            splitPosition = findSplitPos(objects,bounds,totalBounds,axis);
            if(splitPosition == -1){
                std::cout << " retry\n";
                numRetries++;
                axis = (axis+1)%3;
            }
            else{
                numRetries = 10;
            }
        }
        if(splitPosition == -1){
            std::cout << "Could not find good split!\n";
            splitPosition = objects.size();
        }
        interiorNode->setDepth(depth);
        m_nodes.push_back(interiorNode);
        std::vector<int> lowerObjects;
        std::vector<int> upperObjects;
        AABoundingBox lowerBounds;
        AABoundingBox upperBounds;
        for (int i = 0; i < splitPosition; i++) {
            if(m_boundEdges[axis][i].m_start){
                lowerObjects.push_back(m_boundEdges[axis][i].m_primNum);
                lowerBounds.extend(m_objects[m_boundEdges[axis][i].m_primNum]);
            }
        }
        for (int i = splitPosition+1; i < objects.size() * 2; i++){
            if(!m_boundEdges[axis][i].m_start){
                upperObjects.push_back(m_boundEdges[axis][i].m_primNum);
                upperBounds.extend(m_objects[m_boundEdges[axis][i].m_primNum]);
            }
        }
        int nextDepth = depth-1;
        interiorNode->setAxis(axis);
        interiorNode->setPosition(m_boundEdges[axis][splitPosition].m_pos);
        interiorNode->setLowerChildIndex(m_nodes.size());
        buildNode(lowerObjects,bounds,nextDepth,lowerBounds);
        interiorNode->setUpperChildIndex(m_nodes.size());
        buildNode(upperObjects,bounds,nextDepth,upperBounds);
    }
}

char KDTree::findSplitAxis(std::vector<int> &objects, std::vector<AABoundingBox> &bounds, AABoundingBox &totalBounds)
{
    return totalBounds.getMaximumExtent();
}

int KDTree::findSplitPos(std::vector<int> &objects, std::vector<AABoundingBox> &bounds, AABoundingBox &totalBounds, char axis)
{
    float min[3], max[3];
    for(int i = 0; i < objects.size(); i++){
        bounds[objects[i]].getBounds(min,max);
        m_boundEdges[axis][i*2].m_primNum = objects[i];
        m_boundEdges[axis][i*2].m_start = true;
        m_boundEdges[axis][i*2].m_pos = min[axis];
        m_boundEdges[axis][i*2+1].m_primNum = objects[i];
        m_boundEdges[axis][i*2+1].m_start = false;
        m_boundEdges[axis][i*2+1].m_pos = max[axis];
    }
    std::sort(&m_boundEdges[axis][0], &m_boundEdges[axis][objects.size()*2]);

    int bestOffset = -1;
    float bestCost = INFINITY;
    float totalSA = totalBounds.surfaceArea();
    float invTotalSA = 1.0f / totalSA;
    Vec3 d = totalBounds.diagonalVec();

    int nBelow = 0;
    int nAbove = objects.size();
    for( int i=0; i < 2*objects.size(); i++){
        BoundEdge *edge = &m_boundEdges[axis][i];
        if(!edge->m_start){
            nAbove--;
        }
        float pos = edge->m_pos;
        if(pos > totalBounds.min(axis) && pos < totalBounds.max(axis)){
            int otherAxis0 = (axis + 1)%3;
            int otherAxis1 = (axis + 2)%3;
            float belowSA = 2.0f * (d[otherAxis0] * d[otherAxis1] + 
                            (pos - totalBounds.min(axis)) *
                            (d[otherAxis0] + d[otherAxis1]));
            float aboveSA = 2.0f * (d[otherAxis0] * d[otherAxis1] + 
                            (totalBounds.max(axis) - pos) *
                            (d[otherAxis0] + d[otherAxis1]));
            float probBelow = belowSA * invTotalSA;
            float probAbove = aboveSA * invTotalSA;
            float emptyBonus = (nAbove == 0 || nBelow == 0) ? m_emptyBonus : 0.0f;
            float cost = m_traversalCost + m_intersectCost * (1.0f - emptyBonus) *
                            (probBelow * nBelow + probAbove * nAbove);
            if(cost < bestCost){
                bestCost = cost;
                bestOffset = i;
            }
        }
        if(edge->m_start){
            nBelow++;
        }
    }
    return bestOffset;
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
        KDTreeNode* nearNode = ray.m_origin[m_axis] < m_position ? m_owner->m_nodes[m_nodeBelowIndex]:m_owner->m_nodes[m_nodeAboveIndex];
        KDTreeNode* farNode  = ray.m_origin[m_axis] < m_position ? m_owner->m_nodes[m_nodeAboveIndex]:m_owner->m_nodes[m_nodeBelowIndex];
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
                if(!sr.m_hit){
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
            m_owner->m_nodes[m_nodeBelowIndex]->hit(ray,sr,tMin,tMax);
        }
        if(ray.m_origin[m_axis] > m_position){
            m_owner->m_nodes[m_nodeAboveIndex]->hit(ray,sr,tMin,tMax);
        }
    }
}

KDTreeLeafNode::KDTreeLeafNode(std::vector<int> &objects, KDTree* owner)
{
    m_owner = owner;
    for(int i=0;i<objects.size();i++){
        m_objects.push_back(m_owner->m_objects[objects[i]]);
    }
	m_numObjects = m_objects.size();
	if(m_numObjects > 100){
		std::cout << "num objects:" << m_numObjects << std::endl;
	}
}

void KDTreeLeafNode::hit(Ray &ray, ShadeRec &sr, float tMin, float tMax)
{
    sr.m_hitT = tMax;
    for(int i=0; i<m_numObjects;i++){
        m_objects[i]->hit(ray, sr);
    }
}

BoundEdge::BoundEdge()
{}

bool BoundEdge::operator<(const BoundEdge &other) const
{
    if(m_pos == other.m_pos){
        return(m_start && !other.m_start);
    }
    return m_pos < other.m_pos;
}
