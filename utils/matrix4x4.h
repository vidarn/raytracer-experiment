#ifndef __MATRIX4X4_H__

#define __MATRIX4X4_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include "vec3.h"

class Matrix4x4
{
    public:
        Matrix4x4();
        Matrix4x4(std::ifstream &stream);
        Matrix4x4(float entries[4][4]);
        Matrix4x4(float entries[16]);
        Matrix4x4(Vec3 &vector);
        void setIdentity();
        void setTranslation(float x, float y, float z);
        void setScaling(float x, float y, float z);
        void setRotation(int axis, float angle);
        Matrix4x4 invert();
        Matrix4x4 operator*(Matrix4x4 &other);
        Vec3 multPoint(Vec3 &vec);
        Vec3 multVec3(Vec3 &vec);
		float& operator[](int id){assert(id < 16 && id>=0);return m_entries[id];};

		friend std::ostream& operator<<(std::ostream &out, Matrix4x4 &mat);
    private:
        void multRow(int row, float mult);
        void subMultRow(int srcRow,int destRow, float mult);
        float m_entries[16];
};

#endif
