#include "Camera.h"

Camera::Camera(CameraType type, Vector eye, Vector target, Vector up, double fovDegrees, double viewScale) :
type_(type), eye_(eye), viewScale_(viewScale) {
    forward_ = target - eye ;
    forward_.normalize();
    right_ = forward_.crossProduct(up);
    right_.normalize();
    trueUp_ = right_.crossProduct(forward_);

    
}
