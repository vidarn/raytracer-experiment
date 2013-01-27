#include "material.h"
#include "../scene/scene.h"

Material::Material()
{
    m_color = RGBA(0.4f,0.4f,0.4f,1.0f);
	m_reflectivity = 0.0f;
	m_glossiness = 0.0f;
	m_ior = 1.5f;
	m_texture = 0;
    m_layerReflectionBuffers = new RGBA[1];
    m_layers = new MaterialLayer[1];
    m_layers[0].m_brdf = new LambertBRDF;
    m_layers[0].m_ior = m_ior;
    m_layers[0].m_absorbColor = m_color;
    m_layers[0].m_thickness = 0.0f;
    m_numLayers = 1;
    m_outDirs   = new Vec3[m_numLayers];
    m_outColors = new RGBA[m_numLayers];
    m_layerWeights = new float[m_numLayers];
}

Material::Material(std::ifstream &stream)
{
    m_color = RGBA(stream);
	m_color[0] = pow(m_color[0],2.2f);
	m_color[1] = pow(m_color[1],2.2f);
	m_color[2] = pow(m_color[2],2.2f);
    m_coatingColor = RGBA(stream);
	m_coatingColor[0] = pow(m_coatingColor[0],2.2f);
	m_coatingColor[1] = pow(m_coatingColor[1],2.2f);
	m_coatingColor[2] = pow(m_coatingColor[2],2.2f);
    char metal;
    stream.read( reinterpret_cast<char*>( &metal ), sizeof metal );
    stream.read( reinterpret_cast<char*>( &m_reflectivity ), sizeof m_reflectivity );
    stream.read( reinterpret_cast<char*>( &m_thickness    ), sizeof m_thickness    );
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

    m_numLayers = 2;
    m_layerReflectionBuffers = new RGBA[m_numLayers];
    m_layers = new MaterialLayer[m_numLayers];
    m_layers[0].m_brdf = new TorranceSparrowBRDF(m_glossiness, m_ior);
    m_layers[0].m_ior = m_ior;
    m_layers[0].m_absorbColor = m_coatingColor;
    m_layers[0].m_thickness = m_thickness;
    if(!metal)
        m_layers[1].m_brdf = new LambertBRDF;
    else
        m_layers[1].m_brdf = new TorranceSparrowBRDF(30.0f, 0.0f);
    m_layers[1].m_ior = m_ior;
    m_layers[1].m_absorbColor = m_color;
    m_layers[1].m_thickness = 0.0f;
    
    m_outDirs   = new Vec3[m_numLayers];
    m_outColors = new RGBA[m_numLayers];
    m_layerWeights = new float[m_numLayers];
}
Material::~Material()
{
    for(int i=0;i<m_numLayers;i++){
        delete m_layers[i].m_brdf;
    }
    delete[] m_layers;
    if(m_texture != 0)
        delete m_texture;
    delete[] m_outDirs;
    delete[] m_outColors;
    delete[] m_layerWeights;
}

void Material::setColor(RGBA color)
{
	m_color = color;
}

void Material::getColor(const ShadeRec &sr, RGBA *col)
{
    if(sr.m_brdf->m_type == BRDF_DIFFUSE){
        if(m_texture == 0)
            *col = m_color;
        else
            *col = m_texture->getPixel(sr.m_uv.m_d[0], sr.m_uv.m_d[1]);
    }
    else{
        *col = RGBA(1.0f,1.0f,1.0f,1.0f);
    }
}

void Material::shade(ShadeRec &sr, Scene *scene, Sampler &sampler, Ray *nextRay, RGBA *reflectedMult, float *pdf)
{
    Matrix4x4 normalMatrix(sr.m_normal);
    Matrix4x4 normalMatrixInv = normalMatrix.invert();
    float delta = 0.0001f;
    Vec3 tmp;
    Vec3 hitPos = sr.m_hitPos;
    Vec3 invI = sr.getIncidentDirection();
    invI.invert();
    Vec3 invITrans = normalMatrix.multVec3(invI);
    Vec3 normal(0.0f,0.0f,1.0f);
    Vec3 reflectDirTrans;

    float totalLayerWeight = 0;
    for(int i=0;i<m_numLayers;i++){
        BRDF *brdf = m_layers[i].m_brdf;
        Vec3 inDir  = invITrans;
        m_outColors[i] = RGBA(1.0f,1.0f,1.0f,1.0f);
        float ior = 1.0f;
        for(int j=0;j<i;j++){
            m_outColors[i] *= 1.0f - brdf->fresnel(fabsf(inDir.m_d[2]), ior, m_layers[j].m_ior);
            inDir  = brdf->snell(inDir,  m_layers[j].m_ior/ior);
            absorb(m_outColors + i, m_layers[j], fabsf(inDir.m_d[2]));
            ior = m_layers[j].m_ior;
        }
        float shadeVal = m_layers[i].m_brdf->sample_f(inDir, &(m_outDirs[i]), pdf, sampler, sr.m_depth);
        for(int j=i-1;j>=0;j--){
            ior = 1.0f;
            if(j>0){
                ior = m_layers[j-1].m_ior;
            }
            absorb(m_outColors + i, m_layers[j], fabsf(m_outDirs[i].m_d[2]));
            m_outDirs[i] = brdf->snell(m_outDirs[i], ior/m_layers[j].m_ior);
        }
        if(*pdf > 0.0f){
            shadeVal *= fabsf(m_outDirs[i].m_d[2]);
            shadeVal /= *pdf;
            shadeVal = std::max(0.0f,shadeVal);
            m_outColors[i] *= shadeVal;
            if(i == m_numLayers - 1){
                m_outColors[i] *= m_layers[i].m_absorbColor;
            }
            m_layerWeights[i] = std::max(m_outColors[i][0], std::max(m_outColors[i][1], m_outColors[i][2]));
            totalLayerWeight += m_layerWeights[i];
        }
        else{
            m_layerWeights[i] = 0.0f;
            m_outColors[i] *= 0.0f;
        }
    }

    float layerPdf = 0.0f;
    float layerSample = totalLayerWeight * sampler.get1DSample();
    int layer = 0;
    float tmpAccum = 0.0f;
    for(int i=0;i<m_numLayers;i++){
        tmpAccum += m_layerWeights[i];
        if(layerSample < tmpAccum){
            layer = i;
            layerPdf = m_layerWeights[i]/totalLayerWeight;
            break;
        }
    }
    *reflectedMult  = m_outColors[layer];
    if(layerPdf > 0.0f)
        *reflectedMult *= 1.0f/layerPdf;
    
    Vec3 reflectDir = normalMatrixInv.multVec3(m_outDirs[layer]);
    reflectDir.normalize();
    tmp = reflectDir * delta;
    tmp = hitPos + tmp;
    *nextRay = Ray(tmp,reflectDir,false);
    nextRay->computePlucker();
    nextRay->m_depth = sr.m_depth+1;

}

void Material::shade(const Vec3 &in, const Vec3 &out, const ShadeRec &sr, RGBA* col, float pdf){
    Matrix4x4 normalMatrix(sr.m_normal);
	Vec3 inTrans  = normalMatrix.multVec3(in );
	Vec3 outTrans = normalMatrix.multVec3(out);

    RGBA accumCol(1.0f,1.0f,1.0f,1.0f);
	RGBA layerCol;
    RGBA totalShade;
    float shade = outTrans.m_d[2];
    float a = shade;
    RGBA debugTmp = *col;
    if(shade > 0.0f){
        for(int i=0;i<m_numLayers;i++){
            MaterialLayer &layer = m_layers[i];
            BRDF *brdf = layer.m_brdf;
            float shade = brdf->f(inTrans, outTrans);
            if(inTrans.m_d[2] < 0.0f || outTrans.m_d[2] < 0.0f)
                shade = 0.0f;
            layerCol = accumCol * shade;
            Vec3 tmpIn  = inTrans;
            Vec3 tmpOut = outTrans;
            for(int j = i-1; j>=0; j--){
                float nextIor = 1.0f;
                if(j>0)
                    nextIor = m_layers[j-1].m_ior;
                layerCol *= 1.0f - brdf->fresnel(fabsf(tmpOut.m_d[2]), nextIor, m_layers[j].m_ior);
                float cosTheta = fabsf(tmpIn.m_d[2]);
                absorb(&layerCol, m_layers[j], cosTheta);
                float eta = nextIor / m_layers[j].m_ior;
                tmpIn  = brdf->snell(tmpIn, eta);
                tmpOut = brdf->snell(tmpOut, eta);
            }
            if(i+1 < m_numLayers){
                MaterialLayer &nextLayer = m_layers[i+1];
                totalShade += layerCol;
                float prevIor = 1.0f;
                if(i>0)
                    prevIor = m_layers[i-1].m_ior;
                accumCol *= 1.0f - brdf->fresnel(fabsf(inTrans.m_d[2]),prevIor,layer.m_ior);
                float eta = nextLayer.m_ior / layer.m_ior;
                inTrans  = brdf->snell(inTrans, eta);
                outTrans = brdf->snell(outTrans,eta);
                absorb(&accumCol, layer, fabsf(inTrans.m_d[2]));
            }
            else{
                totalShade += layerCol * layer.m_absorbColor;
            }
        }
        shade /= pdf;
        shade = std::max(0.0f,shade);
        RGBA tmp = *col;
        *col = *col * totalShade * shade;
    }
    else{
        *col = RGBA(0.0f,0.0f,0.0f,1.0f);
    }
}

void Material::absorb(RGBA *col, MaterialLayer &layer, const float cosTheta)
{
    if(cosTheta > 0.0f){
        for(int i=0;i<3;i++){
            (*col)[i] *= expf(-((1.0f - layer.m_absorbColor[i])*layer.m_thickness*(1.0f/cosTheta)));
        }
    }
    else{
        for(int i=0;i<3;i++){
            (*col)[i] = 0.0f;
        }
    }
}
