#ifndef __RAY_H__
#define __RAY_H__
#include "vec3.h"

class Ray
{
	public:
		Ray(float tMin=0.0, int depth =0):m_origin(),m_dir(),m_tMin(tMin),m_depth(depth){};
		Ray(Vec3 origin, Vec3 dir, float tMin=0.0, int depth=0):m_origin(origin),m_dir(dir),m_tMin(tMin),m_depth(depth){};

		Vec3 getPointAtPos(float t);

		friend std::ostream& operator<<(std::ostream &out, Ray &vec);

		float m_tMin;
		Vec3 m_origin;
		Vec3 m_dir;
		int m_depth;
};

#endif
