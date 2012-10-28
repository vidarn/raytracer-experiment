#ifndef __POINT_H__
#define __POINT_H__
#include "vec3.h"

class Point
{
	public:
		Point():m_vec(){};
		Point(Vec3 &vec):m_vec(vec){};
		Point(double x, double y, double z):m_vec(x,y,z){};
		Point(double d[3]):m_vec(d){};
		~Point(){};
		double x() {return m_vec.x();};
		double y() {return m_vec.y();};
		double z() {return m_vec.z();};
		double& operator[](int id){assert(id < 3 && id>=0);return m_vec[id];};
		double distance(Point &other){return (m_vec - other.m_vec).magnitude();};
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
