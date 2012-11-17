#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../utils/matrix4x4.h"

class Light
{
    public:
        Light(float strength, Matrix4x4 transform);
        float getLightStrength(Vec3 pos);
        Vec3 getLightDirection(Vec3 pos);
        virtual float computeStrength(Vec3 pos) = 0;
        virtual Vec3 computeDirection(Vec3 pos) = 0;
    private:
        Matrix4x4 m_transform;
        Matrix4x4 m_invTransform;
        float m_strength;
};

#endif
