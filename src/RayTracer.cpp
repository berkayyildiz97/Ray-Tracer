
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Vec.h"
#include "Light.h"
#include "Pigment.h"
#include "SurfaceFinish.h"
#include "Object.h"
#include "Sphere.h"
#include "Ray.h"
#include "Triangle.h"

#define M_PI 3.141592653589793
#define INFINITY 1e8
#define MAX_RAY_DEPTH 4

using namespace std;

Vec backgroundColor = Vec(0.5, 0.5, 0.5);

//Returns result of Phong equation
Vec phong(Ray ray, Light light, Vec P, Vec N, SurfaceFinish surface, Pigment pigment, Vec camera) {
    N.normalize();
    Vec L = light.coordinates - P;
    L.normalize();

    Vec R = (N * L) * 2.0 * N - L;

    Vec V = camera - P;
    V.normalize();

    Vec diffuse = light.color * surface.diffuse * max((L.dot(N)), 0.0f);

    Vec H = L + V;
    H.normalize();
    Vec specular = light.color * surface.specular * pow(max((H.dot(N)), 0.0f), surface.shininess);

    float D = (light.coordinates - P).length();
    Vec phongResult = (pigment.color * diffuse + specular) / (light.attenuation.x + light.attenuation.y * D + light.attenuation.z * D * D);
    return phongResult;
}

//Returns color vector for given ray
Vec trace(Ray ray, vector<Object*> objects, int depth, vector<Light> lights, Vec camera, Vec ambientLight) {

    if (depth > MAX_RAY_DEPTH) {
        return backgroundColor;
    }
    double tnear = INFINITY;
    Object* intersectedObject = NULL;
    Vec localC;

    //Finds the closest intersected object for the ray
    for (Object* object : objects) {
        double t;
        if (object->intersect(ray, t)) {
            if (t < tnear + EPSILON) {
                tnear = t;
                intersectedObject = object;
            }
        }
    }

    if (!intersectedObject) {
        return backgroundColor;
    }

    Vec surfaceColor, reflectedColor, refractedColor;

    Vec point;
    Vec normal;
    Sphere* sphere;
    Triangle* triangle;
    if ((sphere = dynamic_cast<Sphere*>(intersectedObject)) != nullptr) {
        point = ray.source + ray.direction * tnear;
        normal = point - sphere->center;
    }
    else if ((triangle = dynamic_cast<Triangle*>(intersectedObject)) != nullptr) {
        point = ray.source + ray.direction * tnear;
        Vec ab = triangle->vertexA - triangle->vertexB;
        Vec ac = triangle->vertexA - triangle->vertexC;
        normal = ab.cross(ac);
    }
    normal.normalize();
    float bias = 1e-4;
    bool isInside = false;
    if (ray.direction.dot(normal) > 0) {
        normal = Vec(-normal.x, -normal.y, -normal.z);
        isInside = true;
    }

    //Casting shadow rays
    for (Light light : lights) {
        bool isVisible = true;
        Vec lightDirection = light.coordinates - point;
        lightDirection.normalize();
        for (Object* object : objects) {
            Ray lightRay = Ray(point + normal * bias, lightDirection);
            tnear = (light.coordinates.x - lightRay.source.x) / lightRay.direction.x;
            double t;
            if (object->intersect(lightRay, t)) {
                if (t < tnear + EPSILON) {
                    tnear = t;
                    isVisible = false;
                    break;
                }
            }
        }

        if (isVisible) {
            surfaceColor = surfaceColor + phong(ray, light, point, normal, intersectedObject->surfaceFinish, intersectedObject->pigment, camera);
        }
    }
    surfaceColor = surfaceColor + ambientLight * intersectedObject->surfaceFinish.ambient * intersectedObject->pigment.color;

    //Reflection
    if (intersectedObject->surfaceFinish.reflectivity > 0) {
        Vec reflect = ray.direction - normal * 2 * ray.direction.dot(normal);
        reflect.normalize();
        reflectedColor = trace(Ray(point + normal * bias, reflect), objects, depth + 1, lights, camera, ambientLight);
    }
    
    //Refraction
    if (intersectedObject->surfaceFinish.refractivity > 0) {
        float ior = intersectedObject->surfaceFinish.ior;
        float eta = isInside ? ior : 1.0 / ior;
        float cos = -normal.dot(ray.direction);
        float k = 1.0 - (1.0 - cos * cos) * eta * eta;
        Vec refract = ray.direction * eta + normal * (eta * cos - sqrt(k));
        refract.normalize();
        refractedColor = trace(Ray(point - normal * bias, refract), objects, depth + 1, lights, camera, ambientLight);
    }
    
    return surfaceColor + reflectedColor * intersectedObject->surfaceFinish.reflectivity + refractedColor * intersectedObject->surfaceFinish.refractivity;
}

int main(int argc, char* argv[])
{
    int lineIndex = 0;

    string outputFileName;
    int width = 0, height = 0, lightsNum = 0, pigmentsNum = 0, surfaceFinishesNum = 0, objectsNum = 0;
    Vec camera, at, up;
    float fovy = 0.0;

    float x, y, z;

    Vec* ambientLight = NULL;
    vector<Light> lightSources;
    vector<Pigment> pigments;
    vector<SurfaceFinish> surfaceFinishes;
    vector<Object*> objects;

    string inputFilename;

    if (argc == 2) {
        inputFilename = argv[1];
    }
    else if (argc > 2) {
        cout << "ERROR: Too many command-line arguments" << endl;
        return 0;
    }
    else {
        cout << "ERROR: Input file name is required as command-line argument" << endl;
        return 0;
    }

    string line;
    ifstream file(inputFilename);

    if (!file.is_open()) {
        cout << "ERROR: Inputfile not found" << endl;
        return 0;
    }

    //Parsing input file
    while (getline(file, line))
    {
        stringstream   linestream(line);

        if (lineIndex == 0) {
            linestream >> outputFileName;
        }
        else if (lineIndex == 1) {
            linestream >> width >> height;
        }
        else if (lineIndex == 2) {
            linestream >> x >> y >> z;
            camera = Vec(x, y, z);
        }
        else if (lineIndex == 3) {
            linestream >> x >> y >> z;
            at = Vec(x, y, z);
        }
        else if (lineIndex == 4) {
            linestream >> x >> y >> z;
            up = Vec(x, y, z);
        }
        else if (lineIndex == 5) {
            linestream >> fovy;
        }
        else if (lineIndex == 6) {
            linestream >> lightsNum;
        }
        else if (lineIndex > 6 && lineIndex <= lightsNum + 6) {
            float x1, y1, z1, x2, y2, z2, x3, y3, z3;
            linestream >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            if (!ambientLight) {
                ambientLight = new Vec(x2, y2, z2);
            }
            else {
                lightSources.push_back(Light(Vec(x1, y1, z1), Vec(x2, y2, z2), Vec(x3, y3, z3)));
            }
        }
        else if (lineIndex == 7 + lightsNum) {
            linestream >> pigmentsNum;
        }
        else if (lineIndex > 7 + lightsNum && lineIndex <= 7 + lightsNum + pigmentsNum) {
            string pigmentType;
            linestream >> pigmentType >> x >> y >> z;
            pigments.push_back(Pigment(pigmentType, Vec(x, y, z)));
        }
        else if (lineIndex == 8 + lightsNum + pigmentsNum) {
            linestream >> surfaceFinishesNum;
        }
        else if (lineIndex > 8 + lightsNum + pigmentsNum && lineIndex <= 8 + lightsNum + pigmentsNum + surfaceFinishesNum) {
            float ambient, diffuse, specular, shininess, reflectivity, refractivity = 0.0, ior = 0.0;
            linestream >> ambient >> diffuse >> specular >> shininess >> reflectivity;
            if (!linestream.str().empty())
                linestream >> refractivity >> ior;
            surfaceFinishes.push_back(SurfaceFinish(ambient, diffuse, specular, shininess, reflectivity, refractivity, ior));
        }
        else if (lineIndex == 9 + lightsNum + pigmentsNum + surfaceFinishesNum) {
            linestream >> objectsNum;
        }
        else if (lineIndex == 9 + lightsNum + pigmentsNum + surfaceFinishesNum) {
            linestream >> objectsNum;
        }
        else if (lineIndex > 9 + lightsNum + pigmentsNum + surfaceFinishesNum && lineIndex <= 9 + lightsNum + pigmentsNum + surfaceFinishesNum + objectsNum) {
            float radius, pigmentIndex, surfaceFinishIndex;
            string objectType;
            linestream >> pigmentIndex >> surfaceFinishIndex >> objectType;
            if (objectType == "sphere") {
                linestream >> x >> y >> z >> radius;
                objects.push_back(new Sphere(pigments[pigmentIndex], surfaceFinishes[surfaceFinishIndex], Vec(x, y, z), radius));
            }
            else if (objectType == "triangle") {
                float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                linestream >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
                objects.push_back(new Triangle(pigments[pigmentIndex], surfaceFinishes[surfaceFinishIndex], Vec(x1, y1, z1), Vec(x2, y2, z2), Vec(x3, y3, z3)));
            }
            else {
                cout << "ERROR: Unknown Object type\n";
            }
        }

        lineIndex++;
    }

    vector<Vec> image;

    float invWidth = 1.0 / width;
    float invHeight = 1.0 / height;
    float aspectRatio = width / height;
    float angle = tan(M_PI * 0.5 * fovy / 180.0);

    //Tracing rays corresponding to each pixel (i, j)
    for (int j = height; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            Vec N = camera - at;
            N.normalize();
            Vec U = up.cross(N);
            U.normalize();
            Vec V = N.cross(U);
            float distance = 1.0;
            float H = angle * 2.0 * distance;
            float W = H * aspectRatio;
            Vec C = camera - N * distance;
            Vec L = C - U * (W / 2.0) - V * (H / 2.0);
            Vec pixel = L + U * i * W / width + V * j * H / height;
            Vec direction = pixel - camera;
            direction.normalize();
            Ray ray = Ray(camera, direction);
            Vec color = trace(ray, objects, 0, lightSources, camera, *ambientLight);
            image.push_back(color);
        }
    }

    //Output image as ppm p6 file
    std::ofstream ofs(outputFileName, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        image.push_back(Vec(0.0, 0.0, 1.0));
        ofs << static_cast<unsigned char>(min(1.0f, image[i].x) * 255) <<
            static_cast<unsigned char>(min(1.0f, image[i].y) * 255) <<
            static_cast<unsigned char>(min(1.0f, image[i].z) * 255);
    }
    ofs.close();
    cout << outputFileName << " is created" << endl;
}