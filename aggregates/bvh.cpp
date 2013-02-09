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
		}
        else{
            float splitCost;
            int splitBucket;
            findSAHSplit(dim,start,end,bboxCentroids,bbox, &splitCost, &splitBucket);
            if(numTriangles > 6 || splitCost < numTriangles){
                BVHTriangleInfo *midPtr = std::partition(&m_buildData[start], &m_buildData[end], CompareToBucket(dim, splitBucket, 12, bboxCentroids));
                int mid = midPtr - &m_buildData[0];
                if(mid == start || mid == end){
                    mid = start+1;
                }
                node->initInterior(dim, recursiveBuild(start,mid,numTotalNodes), recursiveBuild(mid,end,numTotalNodes));
            }
            else{
                createLeafNode(start,end,bbox,node);
            }
        }
	}
	
	return node;
}

void BVH::findSAHSplit(int dim, int start, int end, AABoundingBox &bboxCentroids, AABoundingBox &bbox, float *splitCost, int *splitBucket)
{
    const int numBuckets = 12;
    SAHBucket buckets[numBuckets];
    for(int i=start; i<end; i++){
        BVHTriangleInfo &node = m_buildData[i];
        int b = numBuckets * (node.m_centroid[dim] - bboxCentroids.min(dim) ) /
            (bboxCentroids.max(dim) - bboxCentroids.min(dim));
        if(b == numBuckets)
            b = numBuckets - 1;
        SAHBucket &bucket = buckets[b];
        bucket.m_bbox.join(node.m_bbox);
        bucket.m_numTris++;
    }

    float cost[numBuckets-1];
    for(int i=0; i<numBuckets-1; i++){
        AABoundingBox b0, b1;
        float c0, c1;
        c0 = c1 = 0.0f;
        for(int j=0; j<i; j++){
            b0.join(buckets[j].m_bbox);
            c0 += buckets[j].m_numTris;
        }
        for(int j=i+1; j<numBuckets; j++){
            b1.join(buckets[j].m_bbox);
            c1 += buckets[j].m_numTris;
        }
        float sa0 = c0 > 0 ?b0.surfaceArea() : 0;
        float sa1 = c1 > 0 ?b1.surfaceArea() : 0;
        cost[i] = 0.5 + (c0 * sa0 + c1 * sa1)/bbox.surfaceArea();
    }

    float minCost = cost[0];
    int minCostSplit = 0;
    for(int i=1;i<numBuckets-1;i++){
        if(cost[i]<minCost){
            minCost = cost[i];
            minCostSplit = i;
        }
    }

    *splitCost = minCost;
    *splitBucket = minCostSplit;
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

bool CompareToBucket::operator()(const BVHTriangleInfo &a) const
{
    int b = m_numBuckets * (a.m_centroid[m_dim] - m_centroidBounds.min(m_dim)) / 
                        (m_centroidBounds.max(m_dim) - m_centroidBounds.min(m_dim));
    if(b == m_numBuckets)
        b--;
    return b <= m_bucket;
}
