#include "Camera.h"

#include <numbers>
#include <cmath>

Camera::Camera(CameraType type, Vector eye, Vector target, Vector up, double fovDegrees, double viewScale) :
type_(type), eye_(eye), viewScale_(viewScale) {
    forward_ = (target - eye).normalized(); ;
    right_ = forward_.crossProduct(up).normalized();
    trueUp_ = right_.crossProduct(forward_);
    
    fovRadians_ = fovDegrees * (std::numbers::pi / 180.0);

    
}

Ray Camera::generateRay(int x, int y, int imageWidth, int imageHeight, double sampleX, double sampleY) const {
    auto u = ( x + sampleX ) / static_cast<double>(imageWidth);
    auto v = ( y + sampleY ) / static_cast<double>(imageHeight);
    auto aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    
    switch(type_) {
        case CameraType::PERSPECTIVE: {
            auto px = ( 2.0 * u - 1.0) * aspectRatio * std::tan(fovRadians_ * 0.5);
            auto py = ( 1.0 - 2.0 * v) * std::tan(fovRadians_ * 0.5);
            
            auto direction = (forward_ + right_ * px + trueUp_ * py).normalized();
            return {eye_, direction};
        }
        case CameraType::ORTHOGRAPHIC: {
            auto screenX = ( 2.0 * u - 1.0) * aspectRatio * viewScale_;
            auto screenY = ( 1.0 - 2.0 * v) * viewScale_;
            
            auto origin = eye_ + right_ * screenX + trueUp_ * screenY;
            auto direction = forward_;
            
            return {origin, direction};
        }
        default:
            return {eye_, forward_};
    }
}
