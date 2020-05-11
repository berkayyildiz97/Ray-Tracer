#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "Vec.h"

#define EPSILON 0.01

class Sphere :
	public Object
{
public:
	float radius;
	Vec center;
	Sphere() = default;
	Sphere(Pigment pigment, SurfaceFinish surfaceFinish, Vec center, float radius) {
		this->pigment = pigment;
		this->surfaceFinish = surfaceFinish;
		this->center = center;
		this->radius = radius;
	}
	//Return true and sets t if the sphere intersects given ray 
	bool intersect(Ray ray, double& t) {
		Vec l = center - ray.source;
		double b = -2.0 * ray.direction.dot(l);
		double a = ray.direction.dot(ray.direction);
		double c = l.dot(l) - radius * radius;
		double delta = pow(b, 2) - 4 * a * c;
		if (delta < 0)
			return false;
		double t1 = (-b - sqrt(delta)) / (2 * a);
		double t2 = (-b + sqrt(delta)) / (2 * a);
		if (t1 >= EPSILON && t2 >= EPSILON) {
			t = min(t1, t2);
		}
		else if (t1 >= EPSILON && t2 < EPSILON) {
			t = t1;
		}
		else if (t1 < EPSILON && t2 >= EPSILON) {
			t = t2;
		}
		else {
			return false;
		}
		return true;
	}
};

#endif