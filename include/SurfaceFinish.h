#ifndef SURFACE_FINISH_H
#define SURFACE_FINISH_H

class SurfaceFinish
{
public:
	float ambient = 0.0, diffuse = 0.0, specular = 0.0, shininess = 0.0, reflectivity = 0.0, refractivity = 0.0, ior = 0.0;
	SurfaceFinish() = default;
	SurfaceFinish(float ambient, float diffuse, float specular, float shininess, float reflectivity, float refractivity, float ior) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
		this->reflectivity = reflectivity;
		this->refractivity = refractivity;
		this->ior = ior;
	}
};

#endif