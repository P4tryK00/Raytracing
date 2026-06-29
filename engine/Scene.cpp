#include "Scene.h"

Scene::Scene(const Color &backgroundColor) : backgroundColor(backgroundColor) {}



void Scene::addPlane(const Plane &plane) {
    planes.push_back(plane);
}

void Scene::addSphere(const Sphere &sphere) {
    spheres.push_back(sphere);
}

void Scene::addTriangle(const Triangle &triangle) {
    triangles.push_back(triangle);
}

// Rejestruje źródło światła 
void Scene::addLight(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

// Udostępnia listę świateł
const std::vector<std::shared_ptr<Light>>& Scene::getLights() const {
    return lights;
}

// Przeprowadza test intersekcji danego promienia z absolutnie każdym obiektem na scenie.
IntersectionResult Scene::intersect(const Ray &ray, double t_min, double t_max) const {
    IntersectionResult closestHit;
    closestHit.type = MISS;

    auto closestT = t_max;


    for (const auto& sphere : spheres) {
        auto hit = sphere.intersect(ray, t_min, closestT);
        if (hit.type != MISS && hit.distance < closestT) {
            closestT = hit.distance; 
            closestHit = hit;        
        }
    }

    for (const auto& triangle : triangles) {
        auto hit = triangle.intersect(ray, t_min, closestT);
        if (hit.type != MISS && hit.distance < closestT) {
            closestT = hit.distance;
            closestHit = hit;
        }
    }

    for (const auto& plane : planes ) {
        auto hit = plane.intersect(ray, t_min, closestT);
        if (hit.type != MISS && hit.distance < closestT) {
            closestT = hit.distance;
            closestHit = hit;
        }
    }

    return closestHit;
}