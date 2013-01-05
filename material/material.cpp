#include "material.h"
#include "../scene/scene.h"

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
}

void Material::setColor(RGBA color)
{
	m_color = color;
}

RGBA Material::shade(ShadeRec shadeRec, Scene *scene, Sampling &sampling)
{
    Matrix4x4 normalMatrix(shadeRec.m_normal);
    Matrix4x4 normalMatrixInv = normalMatrix.invert();
	RGBA col;
	BRDF *brdf;
    float pdf;
    float delta = 0.0001f;
    Vec3 tmp;
    Vec3 hitPos = shadeRec.m_hitPos;
    Vec3 invI = shadeRec.getIncidentDirection();
    invI.invert();
    Vec3 invITrans = normalMatrix.multVec3(invI);
    Vec3 normal(0.0f,0.0f,1.0f);
    LambertBRDF lambert;
    SpecularReflectionBRDF spec(m_ior);
    PhongBRDF phong(m_glossiness);
    TorranceSparrowBRDF torranceSparrow(m_glossiness,m_ior);
    if(m_reflectivity > 0.9f){
        brdf = &torranceSparrow;
    }
    else{
        brdf = &lambert;
    }

	int lightSamples = scene->m_settings.m_lightSamples;
	int lightBounces = scene->m_settings.m_lightBounces;

	// calculate path contribution
	for(int i=0;i< lightSamples;i++){
		RGBA tmp_color;
		int lightIndex = floor(sampling.get1DSample(i+(shadeRec.m_depth*lightSamples))[0]*scene->m_lights.size());
		Light *light = scene->m_lights[lightIndex];
		Vec3 lightDirection = light->getLightDirection(shadeRec, sampling);
        Vec3 lightDirectionTrans = normalMatrix.multVec3(lightDirection);
		float lightStrength = light->getLightStrength(hitPos);
        float shade = lightDirectionTrans.dot(normal);
		shade *= brdf->f(invITrans, lightDirectionTrans);
		shade *= lightStrength;
		shade *= float(scene->m_lights.size())/scene->m_settings.m_lightSamples;
		if(shade > 0.0f){
            tmp = lightDirection * delta;
			tmp = hitPos + tmp;
			Ray ray = Ray(tmp,lightDirection,true);
			ray.computePlucker();
			ray.m_depth = shadeRec.m_depth+1;
			float shadow = scene->traceShadow(ray);
			shade *= shadow;
            tmp_color = light->m_color * shade;
            col += tmp_color;
		}
	}

	if(shadeRec.m_depth+1 < lightBounces){
		// continue path
		Vec3 reflectDirTrans;
        float shade = brdf->sample_f(invITrans, &reflectDirTrans, &pdf, sampling,shadeRec.m_depth);
        Vec3 reflectDir = normalMatrixInv.multVec3(reflectDirTrans);
        shade *= reflectDirTrans.dot(normal);
        shade /= pdf;
        if(shade > 0.0f){
            tmp = reflectDir * delta;
            tmp = hitPos + tmp;
            Ray ray = Ray(tmp,reflectDir,false);
            ray.computePlucker();
            ray.m_depth = shadeRec.m_depth+1;
            RGBA reflected_col = scene->trace(ray,sampling);
            reflected_col *= shade;
            col += reflected_col;
        }
	}
    
    if(m_texture != 0)
        col *= m_texture->getPixel(shadeRec.m_uv[0], shadeRec.m_uv[1]);
    else
        col *= m_color;
    
	return col;
}
