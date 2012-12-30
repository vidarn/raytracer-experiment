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
    std::cout << "glossiness: " << m_glossiness << std::endl;
}

void Material::setColor(RGBA color)
{
	m_color = color;
}

RGBA Material::shade(ShadeRec shadeRec, Scene *scene, Sampling &sampling)
{
	RGBA col;
	Lambert lambert;
    float delta;
    Vec3 tmp;
    Vec3 hitPos = shadeRec.m_hitPos;

    if(m_reflectivity < 1.0f){
		if(scene->m_lights.size() > 0){
			for(int i=0;i< scene->m_settings.m_lightSamples;i++){
				float fLightIndex = sampling.get1DSample(i)[0];
				int lightIndex = floor(fLightIndex*scene->m_lights.size());
				Light *light = scene->m_lights[lightIndex];

				Vec3 lightDirection = light->getLightDirection(shadeRec, sampling);
				float lightStrength = light->getLightStrength(hitPos);
				float shade = lambert.shade(shadeRec, lightDirection);
				shade *= 1-m_reflectivity;
				shade *= lightStrength;
				shade *= float(scene->m_lights.size())/scene->m_settings.m_lightSamples;

				if(!(shade == 0.0f)){
					delta = 0.0001f;

					tmp = lightDirection * delta;
					tmp = hitPos + tmp;
					Ray ray = Ray(tmp,lightDirection,true);
					ray.computePlucker();
					ray.m_depth = shadeRec.m_depth+1;
					float shadow = scene->traceShadow(ray);
					shade *= shadow;
				}

				col += m_color*shade;
			}
		}
    }


    if(m_reflectivity > 0.0f){
        Vec3 invI = shadeRec.getIncidentDirection();
        invI.invert();
        Vec3 reflectDir = shadeRec.getNormal().reflect(invI);
        reflectDir = sampling.getHemisphereSample(0,m_glossiness,reflectDir);
        tmp = reflectDir * delta;
        tmp = hitPos + tmp;
        reflectDir.normalize();
        Ray ray = Ray(tmp,reflectDir,false);
        ray.computePlucker();
        ray.m_depth = shadeRec.m_depth+1;
        RGBA reflected_col = scene->trace(ray,sampling);
        reflected_col *= m_reflectivity;
        col = col + reflected_col;
    }


	return col;
}
