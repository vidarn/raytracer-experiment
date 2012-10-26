#ifndef __RAY_H__
#define __RAY_H__
#include "vec3.h"
#include "point.h"

class Ray
{
	public:
		Ray(double tMin=0.0, int depth =0):m_origin(),m_dir(),m_tMin(tMin),m_depth(depth){};
		Ray(Point origin, Vec3 dir, double tMin=0.0, int depth=0):m_origin(origin),m_dir(dir),m_tMin(tMin),m_depth(depth){};
		Ray(Vec3 dir, double tMin=0.0, int depth=0):m_origin(),m_dir(dir),m_tMin(tMin),m_depth(depth){};
		Ray(Point origin, double tMin=0.0, int depth=0):m_origin(origin),m_dir(),m_tMin(tMin),m_depth(depth){};

		friend std::ostream& operator<<(std::ostream &out, Ray &vec);

		double m_tMin;
		Point m_origin;
		Vec3 m_dir;
		int m_depth;
};

#endif
