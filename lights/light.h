#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../utils/matrix4x4.h"
#include "../utils/ray.h"
#include "../sampler/sampler.h"
#include "../shadeRec/shadeRec.h"

class Light
{
    public:
        Light(){};
        Light(float strength);
        RGBA getLightColor(Vec3 &dir, Vec3 &point, Vec3 &samplePos, float *pdf);
        Vec3 getLightDirection(ShadeRec &sr, Sampler &sampler);
        virtual float computeStrength(Vec3 &dir, Vec3 &point, Vec3 &samplePos, float *pdf) = 0;
        virtual Vec3 computeDirection(ShadeRec &sr, Sampler &sampler) = 0;
        virtual Ray getRay(Sampler &sampler) = 0;
		RGBA m_color;
    protected:
        float m_strength;
};

#endif
