#ifndef LIGHT_H
#define LIGHT_H

#include "Vec.h"

class Light
{
public:
    Vec coordinates, color, attenuation;
    Light() = default;
    Light(Vec coordinates, Vec color, Vec attenuation) {
        this->coordinates = coordinates;
        this->color = color;
        this->attenuation = attenuation;
    }
    friend std::ostream& operator<<(std::ostream& out, const Light& light)
    {
        out << light.coordinates << " " << light.color << " " << light.attenuation;
        return out;
    }
};

#endif
