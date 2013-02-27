#include "matrix4x4.h"
#include <cstdlib>
#include <string.h>

Matrix4x4 matrixFromVector(const Vec3 &vector)
{
    Matrix4x4 mat;
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
    mat[0][0] = u[0];
    mat[1][0] = u[1];
    mat[2][0] = u[2];
    mat[3][0] = 0;
    mat[0][1] = v[0];
    mat[1][1] = v[1];
    mat[2][1] = v[2];
    mat[3][1] = 0;
    mat[0][2] = vector[0];
    mat[1][2] = vector[1];
    mat[2][2] = vector[2];
    mat[3][2] = 0;
    mat[0][3] = 0;
    mat[1][3] = 0;
    mat[2][3] = 0;
    mat[3][3] = 1;
    return mat;
}

Matrix4x4 matrixFromStream(std::ifstream &stream)
{
    Matrix4x4 mat;
    for(int y=0; y<4; y++){
        for(int x=0; x<4; x++){
            stream.read( reinterpret_cast<char*>( &(mat[x][y]) ), sizeof(float));
        }
    }
    return mat;
}
