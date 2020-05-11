#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"


class Triangle :
	public Object
{
    bool isSameSide(Vec p, Vec a1, Vec
        a2, Vec a3) {
        Vec cp1 = (a2 - a1).cross( p - a1);
        Vec cp2 = (a2 - a1).cross( a3 - a1);
        if (cp1.dot(cp2) >= 0) {
            return true;
        }
        return false;
    }
public:
    Vec vertexA, vertexB, vertexC;
	Triangle() = default;
	Triangle(Pigment pigment, SurfaceFinish surfaceFinish, Vec vertexA, Vec vertexB, Vec vertexC) {
		this->pigment = pigment;
		this->surfaceFinish = surfaceFinish;
        this->vertexA = vertexA;
        this->vertexB = vertexB;
        this->vertexC = vertexC;
	}
    //Return true and sets t if the triangle intersects given ray 
    bool intersect(Ray ray, double& t) {

        Vec ab = vertexA - vertexB;
        Vec ac = vertexA - vertexC;
        Vec as = vertexA - ray.source;
        Vec n = ab.cross(ac);

        double M = ray.direction.dot(n);

        t = as.dot(n) / M;

        Vec p = ray.source + ray.direction * t;
        return isSameSide(p, vertexA, vertexB, vertexC) && isSameSide(p, vertexB, vertexC, vertexA)
            && isSameSide(p, vertexC, vertexA, vertexB);
    }
};

#endif