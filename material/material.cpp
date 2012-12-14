#include "material.h"
#include "../scene/scene.h"

Material::Material(std::ifstream &stream)
{
    m_color = RGBA(stream);
    stream.read( reinterpret_cast<char*>( &m_reflectivity ), sizeof m_reflectivity );
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

    if(scene->m_lights.size() > 0){
        float fLightIndex = sampling.get1DSample(0)[0];
        int lightIndex = floor(fLightIndex*scene->m_lights.size());
        Light *light = scene->m_lights[lightIndex];

        Vec3 lightDirection = light->getLightDirection(hitPos, sampling);
        float lightStrength = light->getLightStrength(hitPos);
        float shade = lambert.shade(shadeRec, lightDirection);
        shade *= 1-m_reflectivity;
        shade *= lightStrength;
        col = m_color*shade;

        delta = 0.001f;

        tmp = lightDirection * delta;
        tmp = hitPos + tmp;
        Ray ray = Ray(tmp,lightDirection,true);
        ray.computePlucker();
        ray.m_depth = shadeRec.m_depth+1;
        float shadow = scene->traceShadow(ray);
        col *= float(scene->m_lights.size());
        col *= shadow;
    }

    Vec3 invI = shadeRec.getIncidentDirection();
    invI.invert();
    Vec3 reflectDir = shadeRec.getNormal().reflect(invI);
    tmp = reflectDir * delta;
    tmp = hitPos + tmp;
    reflectDir.normalize();
    Ray ray = Ray(tmp,reflectDir,false);
	ray.computePlucker();
    ray.m_depth = shadeRec.m_depth+1;

    if(m_reflectivity > 0.0f){
        RGBA reflected_col = scene->trace(ray,sampling);
        reflected_col *= m_reflectivity;
        col = col + reflected_col;
    }


	return col;
}
