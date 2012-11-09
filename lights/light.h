#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../utils/point.h"
#include "../utils/matrix4x4.h"

class Light
{
    public:
        Light(float strength, Matrix4x4 transform);
        float getLightStrength(Point pos);
        Vec3 getLightDirection(Point pos);
        virtual float computeStrength(Point pos) = 0;
        virtual Vec3 computeDirection(Point pos) = 0;
    private:
        Matrix4x4 m_transform;
        Matrix4x4 m_invTransform;
        float m_strength;
};

#endif
