#ifndef OBJECT_H
#define OBJECT_H

#include "Pigment.h"
#include "SurfaceFinish.h"
#include "Ray.h"

class Object
{
public:
	Pigment pigment;
	SurfaceFinish surfaceFinish;
	virtual bool intersect(Ray ray, double& t) {
		return false;
	}
};

#endif