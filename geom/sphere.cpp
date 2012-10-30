#include "sphere.h"
#include "../utils/matrix4x4.h"

ShadeRec Sphere::hit(Ray &ray) const
{
	Point pnt = ray.m_origin;
	Vec3 o = pnt.toVec3();
	double a = ray.m_dir.dot(ray.m_dir);
	double b = 2.0*(ray.m_dir.dot(o));
	double c = o.dot(o) - m_radiusSquared;

	ShadeRec sr;

	double discriminant = b*b - 4*a*c;
	if(discriminant < 0){
		return sr;
	}
	double discSqrt = sqrt(discriminant);
	double q;
	if(b<0.0){
		q = (-b - discSqrt)/2.0;
	}
	else{
		q = (-b + discSqrt)/2.0;
	}
	double t0 = q/a;
	double t1 = c/q;
	if(t1<t0){
		double tmp = t0;
		t0 = t1;
		t1 = tmp;
	}
	if(t1<0.0){
		return sr;
	}
	sr.setColor(m_color);
	sr.setIncidentDirection(ray.m_dir);
	sr.setHit(true);
	if(t0<0.0){
		//intersection at t1
		sr.setHitT(t0);
	}
	else{
		//intersection at t0
		sr.setHitT(t0);
	}

	Point hitPos = ray.getPointAtPos(sr.getHitT());
	Vec3 tmpVec = hitPos.toVec3().getNormalized();
	Normal normal(tmpVec);

	/* temporary shading*/
	RGBA tmpColor;
	Vec3 lightDir(1.0,0.5,-1.0);
	lightDir.normalize();
	Vec3 normalVec = normal.toVec3();
	double lambert = lightDir.dot(normalVec);
	if(lambert < 0.0){
		lambert = 0.0;
	}
	tmpColor = m_color*lambert;
	sr.setColor(tmpColor);
	/* end temporary shading*/
	return sr;
}
