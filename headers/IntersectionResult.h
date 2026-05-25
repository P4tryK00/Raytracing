#ifndef RAYTRACING_INTERSECTIONRESULT_H
#define RAYTRACING_INTERSECTIONRESULT_H
#include "Vector.h"
#include "Color.h"

enum IntersectionType {
    MISS,
    HIT,
    INSIDE_PRIMITIVE
};

struct Material {
    Color baseColor = Color(1.0, 1.0, 1.0); // Bazowy kolor obiektu
    
    // Współczynniki modelu Phonga:
    double Ka = 0.2; // Ambient - odbicie światła otoczenia (zawsze obecne)
    double Kd = 0.8; // Diffuse - rozproszenie światła matowego (zależne od kąta padania)
    double Ks = 0.5; // Specular - odbicie lustrzane (rozbłysk)
    double n  = 50.0; // Shininess (gładkość) - im wyższa wartość, tym rozbłysk jest mniejszy i ostrzejszy
    
    
    Material() = default;
    Material(const Color& color, double ka, double kd, double ks, double shininess)
        : baseColor(color), Ka(ka), Kd(kd), Ks(ks), n(shininess) {}
};

struct IntersectionResult {
    IntersectionType type = MISS;
    double distance = 0.0;
    Vector LPOINT;
    Vector intersectionLPOINTNormal;
    Material material;
};

#endif //RAYTRACING_INTERSECTIONRESULT_H