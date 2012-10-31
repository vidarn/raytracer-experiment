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
	//std::cout << hitPos <<std::endl;
	Vec3 tmpVec = hitPos.toVec3().getNormalized();
	Normal normal(tmpVec);
	sr.setNormal(normal);
	return sr;
}
