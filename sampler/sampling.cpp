#include "sampling.h"
#include <cstdlib>


Sampling::Sampling(int numSamples, int num1DSequences, int numSquareSequences, int numDiskSequences, int numHemisphereSequences, Sampler* sampler)
{
    for(int i=0;i<num1DSequences;i++){
        Vec3 *samples = new Vec3[numSamples];
        sampler->generate1DSequence(samples,numSamples);
        m_1DSequences.push_back(samples);
    }
    for(int i=0;i<numSquareSequences;i++){
        Vec3 *samples = new Vec3[numSamples];
        sampler->generateSquareSequence(samples,numSamples);
        m_squareSequences.push_back(samples);
    }
    for(int i=0;i<numDiskSequences;i++){
        Vec3 *samples = new Vec3[numSamples];
        sampler->generateDiskSequence(samples,numSamples);
        m_diskSequences.push_back(samples);
    }
    for(int i=0;i<numHemisphereSequences;i++){
        Vec3 *samples = new Vec3[numSamples];
        sampler->generateHemisphereSequence(samples,numSamples);
        m_hemisphereSequences.push_back(samples);
    }
    m_currentIndex = 0;
}

Sampling::~Sampling()
{
    for(int i=0;i<m_1DSequences.size();i++){
        delete[] m_1DSequences[i];
    }
    for(int i=0;i<m_squareSequences.size();i++){
        delete[] m_squareSequences[i];
    }
    for(int i=0;i<m_diskSequences.size();i++){
        delete[] m_diskSequences[i];
    }
    for(int i=0;i<m_hemisphereSequences.size();i++){
        delete[] m_hemisphereSequences[i];
    }
}

void Sampling::increaseIndex(){
    m_currentIndex++;
}

Vec3 Sampling::get1DSample(int id)
{
    return m_1DSequences[id][m_currentIndex];
}

Vec3 Sampling::getDiskSample(int id)
{
    return m_diskSequences[id][m_currentIndex];
}

Vec3 Sampling::getSquareSample(int id)
{
    return m_squareSequences[id][m_currentIndex];
}

Vec3 Sampling::getHemisphereSample(int id)
{
    float a = m_hemisphereSequences[id][m_currentIndex][0];
    float b = m_hemisphereSequences[id][m_currentIndex][1];
    Vec3 ret;
    ret[2] = b;
    float r = sqrtf(std::max(0.0f,1.0f - ret[2]*ret[2]));
    float phi = 2 * M_PI * a;
    ret[0] = r * cosf(phi);
    ret[1] = r * sinf(phi);
    return ret;
}

Vec3 Sampling::getHemisphereSample(int id, float p)
{
    float a = m_hemisphereSequences[id][m_currentIndex][0];
    float b = m_hemisphereSequences[id][m_currentIndex][1];
    float cosPhi = cos(2.0f * M_PI * a);
    float sinPhi = sin(2.0f * M_PI * a);
    float cosTheta = pow((1.0f - b), 1.0f / (p + 1.0f));
    float sinTheta = sqrtf(1.0f - cosTheta*cosTheta);
    Vec3 ret;
    ret.m_d[0] = sinTheta * cosPhi;
    ret.m_d[1] = sinTheta * sinPhi;
    ret.m_d[2] = cosTheta;
    return ret;
}

Vec3 Sampling::getHemisphereSample(int id, float p, Vec3 &dir)
{
    Vec3 hemi = getHemisphereSample(id,p);
    Vec3 ret;
    Vec3 up;
    int a = 0;
    if(abs(hemi[1]) < abs(hemi[0]))
        a = 1;
    if(abs(hemi[2]) < abs(hemi[a]))
        a = 2;
    up[a] = 1.0f;
    Vec3 w = dir;
    Vec3 v = dir.cross(up);
    Vec3 u = v.cross(w);
    ret += u * hemi[0];
    ret += v * hemi[1];
    ret += w * hemi[2];
    ret.normalize();
    return ret;
}
