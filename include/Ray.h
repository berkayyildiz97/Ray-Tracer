#ifndef RAY_H
#define RAY_H

#include "Vec.h"

class Ray
{
public:
	Vec source;
	Vec direction;
	Ray() = default;
	Ray(Vec source, Vec direction) {
		this->direction = direction;
		this->source = source;
	}
};

#endif