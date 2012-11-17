#include "sphere.h"
#include "../utils/matrix4x4.h"

Sphere::Sphere(std::istream &stream, Matrix4x4 transform, Material *material)
{
    stream.read( reinterpret_cast<char*>( &m_radius ), sizeof m_radius );
    std::cout << "radius: " << m_radius << std::endl;
    m_radiusSquared = m_radius*m_radius;
    m_transform = transform;
    m_material = material;
}

void Sphere::getBounds(float min[3], float max[3]) const
{
    for (int i = 0; i < 3; i++) {
        min[i] = -m_radius;
        max[i] = m_radius;
    }
}

void Sphere::hit(Ray &ray, ShadeRec &sr) const
{
	Point pnt = ray.m_origin;
	Vec3 o = pnt.toVec3();
	float a = ray.m_dir.dot(ray.m_dir);
	float b = 2.0*(ray.m_dir.dot(o));
	float c = o.dot(o) - m_radiusSquared;

	float discriminant = b*b - 4*a*c;
	if(discriminant < 0){
		return;
	}
	float discSqrt = sqrt(discriminant);
	float q;
	if(b<0.0){
		q = (-b - discSqrt)/2.0;
	}
	else{
		q = (-b + discSqrt)/2.0;
	}
	float t0 = q/a;
	float t1 = c/q;
	if(t1<t0){
		float tmp = t0;
		t0 = t1;
		t1 = tmp;
	}
	if(t1<0.0){
		return;
	}
	sr.setHit(true);
	if(t0<0.0){
		//intersection at t1
		sr.setHitT(t1);
	}
	else{
		//intersection at t0
		sr.setHitT(t0);
	}

	Point hitPos = ray.getPointAtPos(sr.getHitT());
	Vec3 tmpVec = hitPos.toVec3().getNormalized();
	Normal normal(tmpVec);
	sr.setNormal(normal);
    sr.setMaterial(m_material);
}
