#ifndef RAYTRACING_SPOTLIGHT_H
#define RAYTRACING_SPOTLIGHT_H

#include "Light.h"
class SpotLight : public Light {
    public:
    SpotLight(const Vector& position, const Vector& direction, const Color& intensity, double cutoffAngleDegrees,
        double outerCutoffAngleDegrees, double constantAtten = 1.0, double linearAtten = 0.0, double quadAtten = 0.0);

    Vector getDirectionFrom(const Vector &point) const override;
    Color getIntensityAt(const Vector &point) const override;
    double getDistanceFrom(const Vector &point) const override;

private:
    Vector position_;
    Vector direction_;
    Color intensity_;

    double cutoffAngleCos_;
    double outerCutoffAngleCos_;
    double constantAtten_;
    double linearAtten_;
    double quadAtten_;

};


#endif //RAYTRACING_SPOTLIGHT_H
