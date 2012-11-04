#ifndef __NORMAL_H__
#define __NORMAL_H__
#include "vec3.h"

class Normal
{
	public:
		Normal():m_vec(){};
		Normal(Vec3 &vec):m_vec(vec){};
		Normal(double x, double y, double z):m_vec(x,y,z){};
		Normal(double d[3]):m_vec(d){};
		~Normal(){};
		Vec3 toVec3(){return m_vec;};
		void normalize(){m_vec.normalize();};
		void operator+=(Normal &other);
	private:
		Vec3 m_vec;
};

#endif
