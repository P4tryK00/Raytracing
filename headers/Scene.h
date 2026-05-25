#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <vector>
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Color.h"
#include "Ray.h"
#include "IntersectionResult.h"


class Scene {
public: 
    Scene(const Color& backgroundColor);
    
    void addSphere(const Sphere& sphere);
    void addPlane(const Plane& plane);
    void addTriangle(const Triangle& triangle);
    
    IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;
    
    Color backgroundColor;
    
private:
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Triangle> triangles;
};


#endif //RAYTRACING_SCENE_H
