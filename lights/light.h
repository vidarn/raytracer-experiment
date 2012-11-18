#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../utils/matrix4x4.h"

class Light
{
    public:
        Light(){};
        Light(float strength);
        float getLightStrength(Vec3 pos);
        Vec3 getLightDirection(Vec3 pos);
        virtual float computeStrength(Vec3 &pos) = 0;
        virtual Vec3 computeDirection(Vec3 &pos) = 0;
    protected:
        float m_strength;
};

#endif
