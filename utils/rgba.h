#ifndef __RGBA_H__
#define __RGBA_H__
#include <cassert>
#include <iostream>
#include <fstream>

class RGBA
{
	public:
		RGBA(std::ifstream &stream);
		RGBA(){m_col[0] = m_col[1] = m_col[2] = m_col[3] = 0.0;};
		RGBA(float d[4]){m_col[0] = d[0];m_col[1] = d[1];m_col[2] = d[2];m_col[3] = d[3];};
		RGBA(float r, float g, float b, float a){m_col[0] = r;m_col[1] = g;m_col[2] = b;m_col[3] = a;};
		float& operator[](int id){assert(id < 4 && id>=0);return m_col[id];};
		float r()const {return m_col[0];};
		float g()const {return m_col[1];};
		float b()const {return m_col[2];};
		float a()const {return m_col[3];};
		RGBA& operator+=(const RGBA &other);
		RGBA operator+(const RGBA &other);
		RGBA& operator-=(const RGBA &other);
		RGBA operator-(const RGBA &other);
		RGBA& operator*=(const float &other);
		RGBA operator*(const float &other) const;
		RGBA& operator*=(const RGBA &other);
		RGBA operator*(const RGBA &other) const;
		RGBA& operator/=(const RGBA &other);
		RGBA operator/(const RGBA &other) const;
		void clamp(float val=1.0);
		float value();

		friend std::ostream& operator<<(std::ostream &out,const RGBA &rgba);
	private:
		float m_col[4];
};

#endif
