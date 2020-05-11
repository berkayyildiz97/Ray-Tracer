#ifndef VEC_H
#define VEC_H

class Vec
{
public:
    float x, y, z;
    Vec() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    };
    Vec(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    float lengthSquare() {
        return x * x + y * y + z * z;
    }
    float length() {
        return sqrt(x * x + y * y + z * z);
    }
    Vec& normalize() {
        float nor2 = lengthSquare();
        if (nor2 > 0) {
            float invNor = 1 / sqrt(nor2);
            x *= invNor, y *= invNor, z *= invNor;
        }
        return *this;
    }
    Vec operator -(Vec v) {
        return Vec(x - v.x, y - v.y, z - v.z);
    }
    Vec operator +(Vec v) { 
        return Vec(x + v.x, y + v.y, z + v.z);
    }
    Vec operator *(float f) { 
        return Vec(x * f, y * f, z * f); 
    }
    Vec operator *(Vec v) const { 
        return Vec(x * v.x, y * v.y, z * v.z); 
    }
    Vec operator *(float f) const { 
        return Vec(x * f, y * f, z * f); 
    }
    Vec operator /(Vec v) const {
        return Vec(x / v.x, y / v.y, z / v.z);
    }
    Vec operator /(float f) const {
        return Vec(x / f, y / f, z / f);
    }
    bool operator ==(Vec v) {
        return v.x == x && v.y == y && v.z == z;
    }
    Vec cross(Vec v) const {
        return Vec(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    float dot(Vec v) { 
        return x * v.x + y * v.y + z * v.z; 
    }
    friend std::ostream& operator<<(std::ostream& out, const Vec& vec)
    {
        out << "Vec(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return out;
    }
};

#endif