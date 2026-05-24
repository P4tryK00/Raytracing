#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "Ray.h"
#include "Vector.h"

enum class CameraType {
    ORTHOGRAPHIC,
    PERSPECTIVE
};

class Camera {
public:
    Camera(CameraType type, Vector eye, Vector target, Vector up,
           double fovDegrees = 90.0, double viewScale = 1.0);

    Ray generateRay(int x, int y, int imageWidth, int imageHeight,
                    double sampleX, double sampleY) const;

private:
    CameraType type_;

    Vector eye_;
    double fovRadians_;
    double viewScale_;

    Vector forward_;
    Vector right_;
    Vector trueUp_;
};


#endif //RAYTRACING_CAMERA_H
