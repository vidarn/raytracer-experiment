#ifndef __AA_BOUNDING_BOX_H__
#define __AA_BOUNDING_BOX_H__
#include "../geom/triangle.h"

class AABoundingBox
{
	public:
		AABoundingBox();
		AABoundingBox(float min[3], float max[3]);
		AABoundingBox(Triangle *tri);
		void extend(Triangle *object);
        void getBounds(float min[3], float max[3]) const;
		bool testBBox(Ray &ray, float &retTmin, float &retTmax) const;
        bool hit(Ray &ray, Vec3 &invDir, bool *dirIsNegative ) const;
		int getMaximumExtent();
        void clear();
        float surfaceArea();
        Vec3 diagonalVec();
        float min(int axis) const {return m_min[axis];};
        float max(int axis) const {return m_max[axis];};
        Vec3 min() const {return Vec3(m_min[0],m_min[1],m_min[2]);};
        Vec3 max() const {return Vec3(m_max[0],m_max[1],m_max[2]);};
        void setMin(float min[3]);
        void setMax(float max[3]);
        void setMin(Vec3 min);
        void setMax(Vec3 max);
		void join(const AABoundingBox &other);
		void join(const Vec3 &other);
	private:
		float m_min[3];
		float m_max[3];
};

#endif
