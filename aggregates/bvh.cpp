#include "bvh.h"
#include <algorithm>

void BVH::build(std::vector<Triangle *> triangles)
{
	m_buildData.reserve(triangles.size());
	m_triangles = triangles;
	for(int i=0;i<triangles.size();i++){
		m_buildData.push_back(BVHTriangleInfo(i,AABoundingBox(triangles[i])));
	}
    int numTotalNodes = 0;
	BVHBuildNode *buildRoot = recursiveBuild(0,m_buildData.size(),&numTotalNodes);
    m_root = new BVHLinearNode[numTotalNodes];
    int offset = 0;
    flattenTree(buildRoot,&offset);
    recursiveDelete(buildRoot);
    m_buildData.clear();
    std::cout << "size of linear node: " << sizeof(BVHLinearNode) << std::endl;
    //writeObjFile("/tmp/bvh.obj");
}

void BVH::recursiveDelete(BVHBuildNode *node)
{
    if(node->m_numTris == 0){
        recursiveDelete(node->m_children[0]);
        recursiveDelete(node->m_children[1]);
    }
    delete node;
}

int BVH::flattenTree(BVHBuildNode *buildNode, int *offset)
{
    BVHLinearNode *node = &m_root[*offset];
    node->m_bbox = buildNode->m_bbox;
    int tmpOffset = (*offset)++;
    if(buildNode->m_numTris > 0){
        node->m_firstTriOffset = buildNode->m_firstTriOffset;
        node->m_numTris = buildNode->m_numTris;
    }
    else{
        node->m_axis = buildNode->m_splitAxis;
        node->m_numTris = 0;
        flattenTree(buildNode->m_children[0],offset);
        node->m_secondChildOffset = flattenTree(buildNode->m_children[1],offset);
    }
    return tmpOffset;
}

BVHBuildNode *BVH::recursiveBuild(int start, int end, int *numTotalNodes)
{
	AABoundingBox bbox;
	for(int i=start; i<end; i++){
		bbox.join(m_buildData[i].m_bbox);
	}
	BVHBuildNode *node = new BVHBuildNode();
    (*numTotalNodes)++;

	int numTriangles = end - start;

	if(numTriangles == 1){
		createLeafNode(start,end,bbox,node);
	}
	else{
		AABoundingBox bboxCentroids;
		for(int i=start; i<end; i++){
			bboxCentroids.join(m_buildData[i].m_centroid);
		}
		int dim = bboxCentroids.getMaximumExtent();
		if(bboxCentroids.min(dim) == bboxCentroids.max(dim)){
			createLeafNode(start,end,bbox,node);
			return node;
		}
		float middlePos = 0.5f * (bboxCentroids.min(dim) + bboxCentroids.max(dim));
		BVHTriangleInfo *midPtr = std::partition(&m_buildData[start], &m_buildData[end], CompareToMid(dim, middlePos));
		int mid = midPtr - &m_buildData[0];
        if(mid == start || mid == end){
            mid = start+1;
        }
		node->initInterior(dim, recursiveBuild(start,mid,numTotalNodes), recursiveBuild(mid,end,numTotalNodes));
	}
	
	return node;
}

void BVH::createLeafNode(int start, int end, AABoundingBox &bbox, BVHBuildNode *node)
{
	int firstTriOffset = m_orderedTriangles.size();
	for(int i=start; i<end;i++){
		int triangleId = m_buildData[i].m_triangleId;
		m_orderedTriangles.push_back(m_triangles[triangleId]);
	}
	node->initLeaf(firstTriOffset, end - start, bbox);
}

void BVH::hit(Ray &ray, ShadeRec &sr) const
{
    int numNodes = 0;
    int hitTests = 0;
    Vec3 invDir(1.0f/ray.m_dir[0], 1.0f/ray.m_dir[1], 1.0f/ray.m_dir[2]);
    bool dirIsNegative[] = {ray.m_dir[0] < 0.0f, ray.m_dir[1] < 0.0f, ray.m_dir[2] < 0.0f};
    int todo[64];
    int todoOffset = 0;
    int nodeNum    = 0;
    while(true){
        numNodes++;
        const BVHLinearNode *node = &m_root[nodeNum];
        if(node->m_numTris > 0){
            for(int i=0;i<node->m_numTris;i++){
                m_orderedTriangles[node->m_firstTriOffset + i]->hit(ray,sr);
            }
            hitTests++;
            if(todoOffset == 0)
                break;
            nodeNum = todo[--todoOffset];
        }
        else{
            if(node->m_bbox.hit(ray,invDir, dirIsNegative)){
                if(dirIsNegative[node->m_axis]){
                    todo[todoOffset++] = nodeNum+1;
                    nodeNum = node->m_secondChildOffset;
                }
                else{
                    nodeNum = nodeNum+1;
                    todo[todoOffset++] = node->m_secondChildOffset;
                }
            }
            else{
                if(todoOffset == 0)
                    break;
                nodeNum = todo[--todoOffset];
            }
        }
    }
}

void BVH::writeObjFile(const char *filename)
{
    FILE *fp = fopen(filename,"w");
    std::vector<Vec3> points;
    recursiveWriteObjFile(points,0);
    for(int i=0;i<points.size();i++){
        fprintf(fp,"v %f %f %f\n", points[i][0], points[i][1], points[i][2]);
    }
    for(int i=0;i<points.size();i+=8){
        fprintf(fp,"f %d %d %d %d\n",i+1,i+2,i+4,i+3);
        fprintf(fp,"f %d %d %d %d\n",i+5,i+6,i+8,i+7);
        fprintf(fp,"f %d %d %d %d\n",i+1,i+2,i+6,i+5);
        fprintf(fp,"f %d %d %d %d\n",i+3,i+4,i+8,i+7);
    }
    fclose(fp);
}

void BVH::recursiveWriteObjFile(std::vector<Vec3> &points, int offset )
{
    const BVHLinearNode *node = &m_root[offset];
    float min[3], max[3];
    node->m_bbox.getBounds(min,max);
    for(int z=0;z<2;z++){
        for(int y=0;y<2;y++){
            for(int x=0;x<2;x++){
                float zVal = z == 0 ? min[2] : max[2];
                float yVal = y == 0 ? min[1] : max[1];
                float xVal = x == 0 ? min[0] : max[0];
                Vec3 tmpVec = Vec3(xVal,yVal,zVal);
                points.push_back(tmpVec);
            }
        }
    }
    if(node->m_numTris > 0){
    }
    else{
        recursiveWriteObjFile(points, offset+1);
        recursiveWriteObjFile(points, node->m_secondChildOffset);
    }
}