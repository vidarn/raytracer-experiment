#include "material.h"
#include "../scene/scene.h"

Material::Material(OSL::ShadingSystem *shadingSys)
{
    m_shadingSystem = shadingSys;
    shadingSys->ShaderGroupBegin();
    shadingSys->Shader("surface","/home/vidar/code/raytracer/shaders/vn_default",NULL);
    m_shader = shadingSys->state();
    shadingSys->ShaderGroupEnd();
}

Material::Material(std::ifstream &stream,OSL::ShadingSystem *shadingSys)
{
    int len;
    stream.read( reinterpret_cast<char*>( &len            ), sizeof len            );
    char *buffer   = new char[len+1];
    for(int i=0;i<len;i++){
        stream.read( buffer + i , sizeof(char));
    }
    buffer[len] = '\0';
    m_shadingSystem = shadingSys;
    shadingSys->ShaderGroupBegin();
    if(len > 0){
        std::cout << "filename: " << buffer << std::endl;
        shadingSys->Shader("surface",buffer,NULL);
    }
    else{
        shadingSys->Shader("surface","/home/vidar/code/raytracer/shaders/vn_default",NULL);
    }
    m_shader = shadingSys->state();
    shadingSys->ShaderGroupEnd();
    delete[] buffer;
}

Material::~Material()
{
}

void Material::getClosure(const ShadeRec &sr, OSL::ShadingContext *shadingContext, RGBA *col, std::vector<ClosureDesc> &closureList)
{
	OSL::ShaderGlobals sg;
	memset(&sg,0,sizeof(OSL::ShaderGlobals));
	for(int i=0;i<3;i++){
		sg.P[i] = sr.m_hitPos[i];
		sg.Ng[i] = sg.N[i] = sr.m_normal[i];
		sg.I[i] = sr.m_iDir[i];
	}
	sg.u = sr.m_uv[0];
	sg.v = sr.m_uv[1];
    OSL::Color3 accum = OSL::Color3(1.0f);
	if(m_shadingSystem->execute(*shadingContext,*m_shader,sg)){
		OSL::Color3 cw;
		const OSL::ClosureColor *clos;
		clos = sg.Ci;
		OSL::Color3 c = OSL::Color3(1.0f);
        processClosure(clos, c, closureList);
	}
}

void Material::processClosure(const OSL::ClosureColor *clos, OSL::Color3 col, std::vector<ClosureDesc> &closureList){
    OSL::Color3 cw;
    switch(clos->type){
        case OSL::ClosureColor::MUL:
            cw = ((const OSL::ClosureMul*) clos)->weight;
            processClosure(((const OSL::ClosureMul*) clos)->closure, col * cw, closureList);
            break;
        case OSL::ClosureColor::ADD:
            processClosure(((const OSL::ClosureAdd*) clos)->closureA, col, closureList);
            processClosure(((const OSL::ClosureAdd*) clos)->closureB, col, closureList);
            break;
        case OSL::ClosureColor::COMPONENT:
            const OSL::ClosureComponent * comp = (const OSL::ClosureComponent *)clos;
            ClosureDesc desc;
            switch(comp->id){
                case DIFFUSE_ID:
                    desc.m_brdf   = new LambertBRDF;
                    break;
                case TORRANCE_SPARROW_ID:
                    desc.m_brdf   = new TorranceSparrowBRDF(0.0f);
                    break;
                case SPECULAR_ID:
                    desc.m_brdf   = new SpecularReflectionBRDF();
                    break;
            }
            desc.m_data   = comp->data();
            desc.m_col    = col;
            desc.m_weight = col[0] + col[1] + col[2];
            closureList.push_back(desc);
            break;
    }
}

void Material::shade(ShadeRec &sr, Scene *scene, Sampler &sampler, Ray *nextRay,
        RGBA *reflectedMult, OSL::ShadingContext *shadingContext)
{
    // get closure list
    std::vector<ClosureDesc> closureList;
    getClosure(sr, shadingContext, reflectedMult, closureList);

    // choose closure to sample
    float totalWeight = 0.0f;
    for(int i=0; i<closureList.size(); i++){
        totalWeight += closureList[i].m_weight;
        closureList[i].m_accumWeight = totalWeight;
    }
    const void *data;
    BRDF *brdf;
    float sample = sampler.get1DSample();
    sample *= totalWeight;
    bool done = false;
    for(int i=0; i<closureList.size(); i++){
        ClosureDesc &desc = closureList[i];
        if(sample <= desc.m_accumWeight && !done){
            brdf = desc.m_brdf;
            data = desc.m_data;
            *reflectedMult = RGBA(desc.m_col[0],desc.m_col[1],desc.m_col[2],1.0f);
            float pdf = desc.m_weight / totalWeight;
            *reflectedMult /= pdf;
            done = true;
        }
        else{
            delete desc.m_brdf;
        }
    }

    // get normal
    Vec3 normal = sr.m_normal;
    //brdf->extractNormal(data, &normal);
    Matrix4x4 normalMatrix(normal);
    Matrix4x4 normalMatrixInv = normalMatrix.invert();
    Vec3 invI = sr.getIncidentDirection();
    invI *= -1.0f;
    Vec3 invITrans = normalMatrix.multVec3(invI);

    // sample brdf
    float pdf;
    Vec3 outDir;
    float shade = brdf->sample_f(invITrans, &outDir, &pdf, sampler, data);
    shade *= outDir[2];
    shade /= pdf;
    if(pdf <= 0.0f || shade < 0.0f)
        shade = 0.0f;
    *reflectedMult *= shade;

    
    // construct next ray
    float delta = 0.0001f;
    Vec3 reflectDir = normalMatrixInv.multVec3(outDir);
    reflectDir.normalize();
    Vec3 tmp = reflectDir * delta;
    tmp = sr.m_hitPos + tmp;
    *nextRay = Ray(tmp,reflectDir,false);
    nextRay->computePlucker();
    nextRay->m_depth = sr.m_depth+1;
    delete brdf;
}

void Material::shade(const Vec3 &in, const Vec3 &out, const ShadeRec &sr, RGBA* col,
        OSL::ShadingContext *shadingContext)
{
    Matrix4x4 normalMatrix(sr.m_normal);
	Vec3 inTrans  = normalMatrix.multVec3(in );
	Vec3 outTrans = normalMatrix.multVec3(out);
    float shade = outTrans[2];
    std::vector<ClosureDesc> closureList;
    getClosure(sr, shadingContext, col, closureList);

    RGBA accum = RGBA(0.0f);
    for(int i=0; i<closureList.size(); i++){
        ClosureDesc &desc = closureList[i];
        accum += RGBA(desc.m_col[0],desc.m_col[1],desc.m_col[2],1.0f) * desc.m_brdf->f(inTrans, outTrans, desc.m_data);
        delete desc.m_brdf;
    }

    *col *= accum * shade;
}
