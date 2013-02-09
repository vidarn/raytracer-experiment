#include "matrix4x4.h"
#include <cstdlib>
#include <string.h>

Matrix4x4::Matrix4x4()
{
	setIdentity();
}

Matrix4x4::Matrix4x4(std::ifstream &stream)
{
    for(int i=0; i<16; i++){
        stream.read( reinterpret_cast<char*>( &(m_entries[i]) ), sizeof m_entries[i] );
    }
}

Matrix4x4::Matrix4x4(float entries[4][4])
{
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			m_entries[x+y*4] = entries[y][x];
		}
	}
}

Matrix4x4::Matrix4x4(float entries[16])
{
	for (int i = 0; i < 16; i++) {
		m_entries[i] = entries[i];
	}
}

Matrix4x4::Matrix4x4(const Vec3 &vector)
{
    Vec3 up;
    int a = 0;
    if(abs(vector[1]) < abs(vector[0]))
        a = 1;
    if(abs(vector[2]) < abs(vector[a]))
        a = 2;
    up[a] = 1.0f;
    Vec3 u = vector.cross(up);
	u.normalize();
    Vec3 v = vector.cross(u);
	v.normalize();
    a = 0;
    m_entries[a + 0] = u[0];
    m_entries[a + 1] = u[1];
    m_entries[a + 2] = u[2];
    m_entries[a + 3] = 0;
    a += 4;
    m_entries[a + 0] = v[0];
    m_entries[a + 1] = v[1];
    m_entries[a + 2] = v[2];
    m_entries[a + 3] = 0;
    a += 4;
    m_entries[a + 0] = vector[0];
    m_entries[a + 1] = vector[1];
    m_entries[a + 2] = vector[2];
    m_entries[a + 3] = 0;
    a += 4;
    m_entries[a + 0] = 0;
    m_entries[a + 1] = 0;
    m_entries[a + 2] = 0;
    m_entries[a + 3] = 1;
}

void Matrix4x4::setTranslation(float x, float y, float z)
{
	setIdentity();
	m_entries[3] = x;
	m_entries[7] = y;
	m_entries[11] = z;
}

void Matrix4x4::setScaling(float x, float y, float z)
{
	setIdentity();
	m_entries[0] = x;
	m_entries[5] = y;
	m_entries[10] = z;
}

void Matrix4x4::setRotation(int axis, float angle)
{
	setIdentity();
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    switch(axis){
        case 0:
            m_entries[1+4] = cosAngle;
            m_entries[2+4] = -sinAngle;
            m_entries[1+8] = sinAngle;
            m_entries[2+8] = cosAngle;
            break;
        case 1:
            m_entries[0] = cosAngle;
            m_entries[2] = sinAngle;
            m_entries[8] = -sinAngle;
            m_entries[2+8] = cosAngle;
            break;
        case 2:
            m_entries[0] = cosAngle;
            m_entries[1] = -sinAngle;
            m_entries[4] = sinAngle;
            m_entries[4+1] = cosAngle;
            break;
        default:
            //raise error ;)
            assert(false);
    }
}

void Matrix4x4::setIdentity()
{
	for(int y=0;y<4;y++){
		for(int x=0;x<4;x++){
			m_entries[x+y*4] = (x==y);
		}
	}
}

Matrix4x4 Matrix4x4::invert()
{
	// Shamelessy stolen from PBRT
    int indxc[4], indxr[4];
    int ipiv[4] = { 0, 0, 0, 0 };
    float minv[4][4];
    memcpy(minv, m_entries, 4*4*sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = -1, icol = -1;
        float big = 0.;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (fabsf(minv[j][k]) >= big) {
                            big = float(fabsf(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    }
                    else if (ipiv[k] > 1)
						std::cerr << "Singular matrix in MatrixInvert";
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k)
                std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.)
			std::cerr << "Singular matrix in MatrixInvert";

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1.f / minv[icol][icol];
        minv[icol][icol] = 1.f;
        for (int j = 0; j < 4; j++)
            minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++)
                    minv[j][k] -= minv[icol][k]*save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix4x4(minv);
}

void Matrix4x4::multRow(int row, float mult)
{
	for(int x = 0; x < 4; x++){
		m_entries[x+row*4] *= mult;
	}
}

void Matrix4x4::subMultRow(int srcRow,int destRow, float mult)
{
	for(int x = 0; x < 4; x++){
		m_entries[x+destRow*4] -= mult*m_entries[x+srcRow*4];
	}
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 &other)
{
	Matrix4x4 result;
	for(int y=0;y<4;y++){
		for(int x=0;x<4;x++){
			float val = 0;
			for (int i = 0; i < 4; i++) {
				val += m_entries[x+i*4]*other.m_entries[i+y*4];
			}
			result.m_entries[x+y*4] = val;
		}
	}
	return result;
}

Vec3 Matrix4x4::multVec3(const Vec3 &vec)
{
	Vec3 result;
	for(int y=0;y<3;y++){
		float val = 0;
		for(int x=0;x<3;x++){
			val += m_entries[x+y*4]*vec[x];
		}
		result[y] = val;
	}
	return result;
}

Vec3 Matrix4x4::multPoint(const Vec3 &point)
{
	Vec3 result = multVec3(point);
	for(int y=0;y<3;y++){
		result[y] += m_entries[3+y*4];
	}
	return result;
}

std::ostream& operator<<(std::ostream &out, Matrix4x4 &mat)
{
	out << "M";
	for(int y=0;y<4;y++){
		if(y>0){
			out << " ";
		}
		out << "[";
		for(int x=0;x<4;x++){
			if(x>0){
				out << ", ";
			}
			out << mat.m_entries[x+y*4];
		}
		out << "]" << std::endl;
	}
	return out;
}
