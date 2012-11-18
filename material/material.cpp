#include "material.h"
#include "../scene/scene.h"

void Material::setColor(RGBA color)
{
	m_color = color;
}

RGBA Material::shade(ShadeRec shadeRec, Light *light, Scene *scene)
{
	RGBA col;
	Lambert lambert;

	Vec3 hitPos = shadeRec.getHitPos();
	Vec3 lightDirection = light->getLightDirection(hitPos);
    float lightStrength = light->getLightStrength(hitPos);
	float shade = lambert.shade(shadeRec, lightDirection);
	shade *= 0.8;
    shade *= lightStrength;
	col = m_color*shade;

	Phong phong;
	shade = phong.shade(shadeRec, lightDirection);
	RGBA specular_col(1.0,1.0,1.0,1.0);
	shade *= 0.2;
    shade *= lightStrength;
	specular_col *= shade;
	col = col + specular_col;

    float delta = 0.0001f;
    Vec3 tmp = lightDirection * delta;
    hitPos += tmp;
    Ray ray(hitPos,lightDirection);
    ray.m_depth = 5;
    float shadow = scene->traceShadow(ray);
    col *= shadow;

	return col;
}
