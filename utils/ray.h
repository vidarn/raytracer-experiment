#ifndef __RAY_H__
#define __RAY_H__
#include "vec3.h"
#include "point.h"

class Ray
{
	public:
		Ray(float tMin=0.0, int depth =0):m_origin(),m_dir(),m_tMin(tMin),m_depth(depth){};
		Ray(Point origin, Vec3 dir, float tMin=0.0, int depth=0):m_origin(origin),m_dir(dir),m_tMin(tMin),m_depth(depth){};
		Ray(Vec3 dir, float tMin=0.0, int depth=0):m_origin(),m_dir(dir),m_tMin(tMin),m_depth(depth){};
		Ray(Point origin, float tMin=0.0, int depth=0):m_origin(origin),m_dir(),m_tMin(tMin),m_depth(depth){};

		Point getPointAtPos(float t);

		friend std::ostream& operator<<(std::ostream &out, Ray &vec);

		float m_tMin;
		Point m_origin;
		Vec3 m_dir;
		int m_depth;
};

#endif
