#ifndef __POINT_H__
#define __POINT_H__
#include "vec3.h"

class Point
{
	public:
		Point():m_vec(){};
		Point(Vec3 &vec):m_vec(vec){};
		Point(float x, float y, float z):m_vec(x,y,z){};
		Point(float d[3]):m_vec(d){};
		~Point(){};
		float x() {return m_vec.x();};
		float y() {return m_vec.y();};
		float z() {return m_vec.z();};
		float& operator[](int id){assert(id < 3 && id>=0);return m_vec[id];};
		float distance(Point &other){return (m_vec - other.m_vec).magnitude();};
		Vec3 operator-(Point &other);
		Point& operator-=(Vec3 &vec);
		Point operator-(Vec3 &vec);
		Point& operator+=(Vec3 &vec);
		Point operator+(Vec3 &vec);
		Vec3 toVec3(){return m_vec;};

		friend std::ostream& operator<<(std::ostream &out, Point &point);
	private:
		Vec3 m_vec;
};

#endif
