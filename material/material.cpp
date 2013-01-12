#include "material.h"
#include "../scene/scene.h"

Material::Material()
{
    m_color = RGBA(0.4f,0.4f,0.4f,1.0f);
	m_reflectivity = 0.0f;
	m_glossiness = 0.0f;
	m_ior = 1.5f;
	m_texture = 0;
	m_brdf = new LambertBRDF;
}

Material::Material(std::ifstream &stream)
{
    m_color = RGBA(stream);
	m_color[0] = pow(m_color[0],2.2f);
	m_color[1] = pow(m_color[1],2.2f);
	m_color[2] = pow(m_color[2],2.2f);
    stream.read( reinterpret_cast<char*>( &m_reflectivity ), sizeof m_reflectivity );
    stream.read( reinterpret_cast<char*>( &m_glossiness   ), sizeof m_glossiness   );
    stream.read( reinterpret_cast<char*>( &m_ior          ), sizeof m_ior          );
    int len;
    stream.read( reinterpret_cast<char*>( &len            ), sizeof len            );
    char *buffer = new char[len+1];
    for(int i=0;i<len;i++){
        stream.read( buffer + i , sizeof(char));
    }
    buffer[len] = 0;
    if(len > 0)
        m_texture = new Image(buffer);
    else
        m_texture = 0;
    delete buffer;

    if(m_reflectivity > 0.9f){
        m_brdf = new TorranceSparrowBRDF(m_glossiness,m_ior);
    }
    else{
        m_brdf = new LambertBRDF;
    }
}

void Material::setColor(RGBA color)
{
	m_color = color;
}

RGBA Material::shade(ShadeRec sr, Scene *scene, Sampling &sampling, Ray *nextRay, RGBA *reflectedMult, float *pdf)
{
    Matrix4x4 normalMatrix(sr.m_normal);
    Matrix4x4 normalMatrixInv = normalMatrix.invert();
	RGBA col;
    float delta = 0.0001f;
    Vec3 tmp;
    Vec3 hitPos = sr.m_hitPos;
    Vec3 invI = sr.getIncidentDirection();
    invI.invert();
    Vec3 invITrans = normalMatrix.multVec3(invI);
    Vec3 normal(0.0f,0.0f,1.0f);
    Vec3 reflectDirTrans;
    float shadeVal = m_brdf->sample_f(invITrans, &reflectDirTrans, pdf, sampling, sr.m_depth);
    Vec3 reflectDir = normalMatrixInv.multVec3(reflectDirTrans);
    tmp = reflectDir * delta;
    tmp = hitPos + tmp;
    *nextRay = Ray(tmp,reflectDir,false);
    nextRay->computePlucker();
    nextRay->m_depth = sr.m_depth+1;
    if(m_texture == 0)
        *reflectedMult = m_color;
    else
        *reflectedMult = m_texture->getPixel(sr.m_uv[0], sr.m_uv[1]);

    shadeVal *= reflectDirTrans.m_d[2];
    shadeVal /= *pdf;
	shadeVal = std::max(0.0f,shadeVal);

	*reflectedMult *= shadeVal;

	return col;
}

void Material::shade(const Vec3 &in, const Vec3 &out, const ShadeRec &sr, RGBA* col, float pdf){
    Matrix4x4 normalMatrix(sr.m_normal);
	Vec3 inTrans  = normalMatrix.multVec3(in );
	Vec3 outTrans = normalMatrix.multVec3(out);
    float shade = outTrans.m_d[2];
    shade *= m_brdf->f(inTrans, outTrans);
    shade /= pdf;
    shade = std::max(0.0f,shade);
    *col = *col * shade;
    if(m_texture == 0)
        *col *= m_color;
    else
        *col *= m_texture->getPixel(sr.m_uv.m_d[0], sr.m_uv.m_d[1]);
}
