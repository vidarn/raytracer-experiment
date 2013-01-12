#ifndef __RAY_H__
#define __RAY_H__
#include "vec3.h"

class Ray
{
	public:
		Ray(Vec3 origin, Vec3 dir, bool shadow, float tMin=0.0, int depth=0):m_origin(origin),m_dir(dir),m_shadow(shadow),m_tMin(tMin),m_depth(depth){};

		Vec3 getPointAtPos(float t);
        void computePlucker();

		friend std::ostream& operator<<(std::ostream &out, Ray &vec);

        static int numRays;

		float m_tMin;
		Vec3 m_origin;
		Vec3 m_dir;
		int m_depth;
        float m_plucker[6];
        bool m_shadow;
};

#endif
