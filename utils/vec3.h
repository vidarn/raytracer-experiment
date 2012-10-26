#ifndef __VEC3_H__
#define __VEC3_H__
#include <cmath>
#include <cassert>
#include <iostream>

class Vec3
{
	public:
		Vec3() { m_d[0] = m_d[1] = m_d[2] = 0.0;};
		Vec3(double x, double y, double z) { m_d[0] = x; m_d[1] = y; m_d[2] = z;};
		Vec3(double d[3]) { m_d[0] = d[0]; m_d[1] = d[1]; m_d[2] = d[2];};
		~Vec3() {};
		double dot(Vec3 &other);
		Vec3 cross(Vec3 &other);
		double magnitude(){return sqrt(x()*x()+y()*y()+z()*z());};
		void normalize();
		Vec3 getNormalized();
		double x() {return m_d[0];};
		double y() {return m_d[1];};
		double z() {return m_d[2];};
		double& operator[](int id){assert(id < 3 && id>=0);return m_d[id];};
		Vec3& operator+=(Vec3 &other);
		Vec3 operator+(Vec3 &other);
		Vec3& operator-=(Vec3 &other);
		Vec3 operator-(Vec3 &other);

		friend std::ostream& operator<<(std::ostream &out, Vec3 &vec);
	private:
		double m_d[3];
};

#endif
