#include "phong.h"

float Phong::shade(ShadeRec &shadeRec, Vec3 &lightDir) const
{
	Vec3 normalVec = shadeRec.getNormal().toVec3();
	Vec3 reflected = normalVec.reflect(lightDir);
	Vec3 invI = shadeRec.getIncidentDirection();
	invI.invert();
	float val = invI.dot(reflected);
	if(val < 0.0){
		val = 0.0;
	}
	else{
		val = pow(val,10);
	}
	return val;
}
