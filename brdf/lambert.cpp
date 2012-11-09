#include "lambert.h"

float Lambert::shade(ShadeRec &shadeRec, Vec3 &lightDir) const
{
	Vec3 normalVec = shadeRec.getNormal().toVec3();
	float val = lightDir.dot(normalVec);
	if(val < 0.0){
		val = 0.0;
	}
	return val;
}
