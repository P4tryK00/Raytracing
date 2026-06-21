#include "SpotLight.h"
#include <cmath>
#include <numbers>

SpotLight::SpotLight(const Vector &position, const Vector &direction, const Color &intensity,
                    double cutoffAngleDegrees, double outerCutoffAngleDegrees, double constantAtten, double linearAtten,
                    double quadAtten) : position_(position), direction_(direction), intensity_(intensity),
                                        constantAtten_(constantAtten), linearAtten_(linearAtten), quadAtten_(quadAtten) {
    direction_ = direction.normalized();

    cutoffAngleCos_ = std::cos(cutoffAngleDegrees * std::numbers::pi / 180.0);
    outerCutoffAngleCos_ = std::cos(outerCutoffAngleDegrees * std::numbers::pi / 180.0);

}

Vector SpotLight::getDirectionFrom(const Vector &point) const {
    return  (position_ - point).normalized();
}

double SpotLight::getDistanceFrom(const Vector &point) const {
    return (position_ - point).length();
}

Color SpotLight::getIntensityAt(const Vector &point) const {
    auto distance = getDistanceFrom(point);

    auto atteuntion = constantAtten_ + ( linearAtten_ * distance ) + ( quadAtten_ * distance * distance );
    if (atteuntion <= 0.0) atteuntion = 1.0;
    Color baseIntensity = intensity_ / atteuntion;

    Vector lightToPoint = (point - position_).normalized();

    auto theta = lightToPoint.dotProduct(direction_);

    auto epsilon = cutoffAngleCos_ -outerCutoffAngleCos_;

    auto intensityScale = 0.0;

    if (epsilon > 0.0) {
        intensityScale = ( theta - outerCutoffAngleCos_) / epsilon;

        if (intensityScale < 0.0) intensityScale = 0.0;
        if (intensityScale > 1.0) intensityScale = 1.0;
    }else {
        intensityScale = ( theta >= cutoffAngleCos_) ? 1.0 : 0.0;
    }
    return baseIntensity * intensityScale;
}
