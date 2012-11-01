#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../utils/point.h"
#include "../utils/matrix4x4.h"

class Light
{
    public:
        Light(double strength, Matrix4x4 transform);
        double getLightStrength(Point pos);
        Vec3 getLightDirection(Point pos);
        virtual double computeStrength(Point pos) = 0;
        virtual Vec3 computeDirection(Point pos) = 0;
    private:
        Matrix4x4 m_transform;
        Matrix4x4 m_invTransform;
        double m_strength;
};

#endif
