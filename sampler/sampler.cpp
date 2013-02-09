#include "sampler.h"


float Sampler::get1DSample(){
    return getSample();
}

Vec3 Sampler::getDiskSample(){
    Vec3 ret;
    bool found = false;
    while(!found){
        ret[0] = 1.0f - 2.0f*getSample();
        ret[1] = 1.0f - 2.0f*getSample();
        if(sqrtf(ret[0]*ret[0] + ret[1]*ret[1]) <= 1.0f){
            found = true;
        }
    }
    return ret;
}

Vec3 Sampler::getSquareSample(){
    return Vec3(getSample(), getSample(), 0.0f);
}

Vec3 Sampler::getUniformHemisphereSample(){
    float z = getSample();
    float r = sqrtf(std::max(0.0f, 1.0f - z*z));
    float phi = 2.0f * M_PI * getSample();
    float x = r * cosf(phi);
    float y = r * sinf(phi);
    return Vec3(x,y,z);
}

Vec3 Sampler::getCosineHemisphereSample(){
    Vec3 ret = getDiskSample();
    ret[2] = sqrtf(std::max(0.0f, 1.0f - ret[0] * ret[0] - ret[1] * ret[1]));
    return ret;
}
