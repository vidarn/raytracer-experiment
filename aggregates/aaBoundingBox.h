#ifndef __AA_BOUNDING_BOX_H__
#define __AA_BOUNDING_BOX_H__
#include "../geom/triangle.h"

class AABoundingBox
{
	public:
		AABoundingBox(); // NOTE: creates a giant Bounding box, not an infiniesimal one as one could belive
		void extend(Triangle *object);
        void getBounds(float min[3], float max[3]) const;
		bool testBBox(Ray &ray, float &retTmin, float &retTmax) const;
		int getMaximumExtent();
        void clear();
        float surfaceArea();
        Vec3 diagonalVec();
        float min(int axis){return m_min[axis];};
        float max(int axis){return m_max[axis];};
        void setMin(float min[3]);
        void setMax(float max[3]);
	private:
		float m_min[3];
		float m_max[3];
};

#endif
