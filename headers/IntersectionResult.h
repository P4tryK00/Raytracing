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
        Color baseColor = Color(1.0, 1.0, 1.0);
};
struct IntersectionResult {
        IntersectionType type = MISS;
        double distance = 0.0;
        Vector LPOINT;
        Vector intersectionLPOINTNormal;
        
        Material material;
};


#endif //RAYTRACING_INTERSECTIONRESULT_H
