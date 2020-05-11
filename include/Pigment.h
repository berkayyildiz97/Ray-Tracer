#ifndef PIGMENT_H
#define PIGMENT_H

#include <string>

#include "Vec.h"

using namespace std;

class Pigment
{
public:
	string type = "solid";
	Vec color;
    Pigment() = default;
    Pigment(string type, Vec color) {
        if (type != "solid") {
            cout << "ERROR: Unknown Pigment type\n";
        }
        this->type = type;
        this->color = color;
    }
};

#endif
