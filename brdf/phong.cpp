#include "phong.h"

double Phong::shade(ShadeRec &shadeRec, Vec3 &lightDir) const
{
	Vec3 normalVec = shadeRec.getNormal().toVec3();
	Vec3 reflected = normalVec.reflect(lightDir);
	double val = shadeRec.getIncidentDirection().dot(reflected);
	if(val < 0.0){
		val = 0.0;
	}
	return val;
}
