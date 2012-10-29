#ifndef __RGBA_H__
#define __RGBA_H__
#include <cassert>
#include <iostream>

class RGBA
{
	public:
		RGBA(){m_col[0] = m_col[1] = m_col[2] = m_col[3] = 0.0;};
		RGBA(double d[4]){m_col[0] = d[0];m_col[1] = d[1];m_col[2] = d[2];m_col[3] = d[3];};
		RGBA(double r, double g, double b, double a){m_col[0] = r;m_col[1] = g;m_col[2] = b;m_col[3] = a;};
		double& operator[](int id){assert(id < 4 && id>=0);return m_col[id];};
		double r(){return m_col[0];};
		double g(){return m_col[1];};
		double b(){return m_col[2];};
		double a(){return m_col[3];};
		RGBA& operator+=(RGBA &other);
		RGBA operator+(RGBA &other);
		RGBA& operator-=(RGBA &other);
		RGBA operator-(RGBA &other);
		RGBA& operator*=(double &other);
		RGBA operator*(double &other) const;

		friend std::ostream& operator<<(std::ostream &out, RGBA &rgba);
	private:
		double m_col[4];
};

#endif
