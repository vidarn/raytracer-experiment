#ifndef __VEC3_H__
#define __VEC3_H__
#include <cmath>
#include <cassert>
#include <iostream>

class Vec3
{
	public:
		Vec3() { m_d[0] = m_d[1] = m_d[2] = 0.0;};
		Vec3(float x, float y, float z) { m_d[0] = x; m_d[1] = y; m_d[2] = z;};
		Vec3(float d[3]) { m_d[0] = d[0]; m_d[1] = d[1]; m_d[2] = d[2];};
		~Vec3() {};
		float dot(const Vec3 &other) const;
		Vec3 cross(const Vec3 &other) const;
		Vec3 reflect(Vec3 &other);
		float magnitude(){return sqrt(x()*x()+y()*y()+z()*z());};
		void normalize();
		Vec3 getNormalized();
		void invert();
        Vec3 alignTo(Vec3 &other);
		float& x() {return m_d[0];};
		float& y() {return m_d[1];};
		float& z() {return m_d[2];};
		float get(int id) const {return m_d[id];};
		float distance(const Vec3 &other)const {return (operator-(other)).magnitude();};
		float& operator[](int id){return m_d[id];};
		Vec3& operator+=(const Vec3 &other);
		Vec3 operator+(const Vec3 &other)const;
		Vec3& operator-=(const Vec3 &other);
		Vec3 operator-(const Vec3 &other)const;
		Vec3& operator*=(const float &other);
		Vec3 operator*(const float &other) const;

		friend std::ostream& operator<<(std::ostream &out, Vec3 &vec);
		float m_d[3];
	private:
};

#endif
