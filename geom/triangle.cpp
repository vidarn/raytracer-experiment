#include "triangle.h"
#include "mesh.h"
#include <cfloat>
#include <cmath>

void Triangle::calculateNormal()
{
    Vec3 v1 = *m_points[0] - *m_points[1];
    Vec3 v2 = *m_points[1] - *m_points[2];
    m_normal = v1.cross(v2);
    m_normal.normalize();
    for (int i = 0; i < 3; i++) {
        *(m_normals[i]) += m_normal;
    }
    m_distOrigin = -(m_points[0]->dot(m_normal));
    float a = FLT_MIN;
    for (int i = 0; i < 3; i++) {
        if(fabs(m_normal[i]) > a){
            m_maxNormalDir = i;
            a = fabs(m_normal[i]);
        }
    }
}

void Triangle::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = FLT_MAX;
        max[i] = -FLT_MAX;
        for (int j = 0; j < 3; j++){
            Vec3 * tmpPoint = m_points[j];
            float val = (*tmpPoint)[i];
            if(val < min[i]){
                min[i] = val;
            }
            if(val > max[i]){
                max[i] = val;
            }
        }
    }
}

void Triangle::computePlucker()
{
    Vec3 p, q;
    for(int i=0;i < 3;i++){
        p = *m_points[i];
        q = *m_points[(i+1)%3];
        m_plucker[i][0] = p[0] * q[1] - q[0] * p[1];
        m_plucker[i][1] = p[0] * q[2] - q[0] * p[2];
        m_plucker[i][2] = p[0] - q[0];
        m_plucker[i][3] = p[1] * q[2] - q[1] * p[2];
        m_plucker[i][4] = p[2] - q[2];
        m_plucker[i][5] = q[1] - p[1];
    }
    int x = (m_maxNormalDir+1)%3;
    int y = (m_maxNormalDir+2)%3;
    float mat[4];
    mat[0] = (*m_points[0])[x] - (*m_points[2])[x];
    mat[1] = (*m_points[1])[x] - (*m_points[2])[x];
    mat[2] = (*m_points[0])[y] - (*m_points[2])[y];
    mat[3] = (*m_points[1])[y] - (*m_points[2])[y];

    mat[1] /= mat[0];

    m_barry[0] = 1.0f / mat[0];
    m_barry[1] = mat[2];
    m_barry[2] = 1.0f / (mat[3] - (mat[1] * mat[2]));
    m_barry[3] = mat[1];
}

void Triangle::hit(Ray &ray, ShadeRec &sr) const
{
    float sides[3];
    for(int i=0;i < 3;i++){
        sides[i] = ray.m_plucker[0]*m_plucker[i][4] +
                    ray.m_plucker[1]*m_plucker[i][5] +
                    ray.m_plucker[2]*m_plucker[i][3] +
                    ray.m_plucker[3]*m_plucker[i][2] +
                    ray.m_plucker[4]*m_plucker[i][0] +
                    ray.m_plucker[5]*m_plucker[i][1];
    }
    if(sides[0] > 0.0f && sides[1] > 0.0f && sides[2] > 0.0f){
        float t = - (ray.m_origin.dot(m_normal) + m_distOrigin)/ (ray.m_dir.dot(m_normal));
        if(t<sr.m_hitT && t>0.0f){
            sr.m_hit = true;
            sr.m_hitT = t;
            sr.m_triangle = this;
        }
    }
}

void Triangle::shadeInfo(Ray &ray, ShadeRec &sr) const
{
    int x = (m_maxNormalDir+1)%3;
    int y = (m_maxNormalDir+2)%3;
    float b0 = sr.m_hitPos[x] - (*m_points[2])[x];
    float b1 = sr.m_hitPos[y] - (*m_points[2])[y];
    b0 *= m_barry[0];
    b1 -= b0 * m_barry[1];
    b1 *= m_barry[2];
    b0 -= b1*m_barry[3];

    if(isnan(b0)){
        b0 = 0.0f;
    }
    if(isnan(b1)){
        b1 = 0.0f;
    }

    b0 = b0 < 0.0f ? 0.0f : b0;
    b0 = b0 > 1.0f ? 1.0f : b0;
    b1 = b1 < 0.0f ? 0.0f : b1;
    b1 = b1 > 1.0f ? 1.0f : b1;

    sr.setMaterial(m_material);
    Vec3 normal = *(m_normals[0]) * b0;
    normal +=*(m_normals[1]) * b1;
    normal +=*(m_normals[2]) * (1.0f - b1 - b0);
    normal.normalize();
    Vec3 uv = *(m_uvs[0]) * b0;
    uv +=*(m_uvs[1]) * b1;
    uv +=*(m_uvs[2]) * (1.0f - b1 - b0);
    sr.m_uv = uv;
    sr.m_normal = normal;
}
