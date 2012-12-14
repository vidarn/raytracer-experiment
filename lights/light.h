#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../utils/matrix4x4.h"
#include "../sampler/sampling.h"

class Light
{
    public:
        Light(){};
        Light(float strength);
        float getLightStrength(Vec3 pos);
        Vec3 getLightDirection(Vec3 pos, Sampling &sampling);
        virtual float computeStrength(Vec3 &pos) = 0;
        virtual Vec3 computeDirection(Vec3 &pos, Sampling &sampling) = 0;
    protected:
        float m_strength;
};

#endif
