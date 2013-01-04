#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../utils/matrix4x4.h"
#include "../sampler/sampling.h"
#include "../shadeRec/shadeRec.h"

class Light
{
    public:
        Light(){};
        Light(float strength);
        float getLightStrength(Vec3 &pos);
        Vec3 getLightDirection(ShadeRec &sr, Sampling &sampling);
        virtual float computeStrength(Vec3 &pos) = 0;
        virtual Vec3 computeDirection(ShadeRec &sr, Sampling &sampling) = 0;
		RGBA m_color;
    protected:
        float m_strength;
};

#endif
