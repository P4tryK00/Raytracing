#include "Camera.h"

#include <numbers>
#include <cmath>

Camera::Camera(CameraType type, Vector eye, Vector target, Vector up, double fovDegrees, double viewScale) :
type_(type), eye_(eye), viewScale_(viewScale) {

    // Znormalizowany wektor celowania (oś Z kamery).
    forward_ = (target - eye).normalized();

    // Wektor "w prawo" (oś X kamery). Liczony jako iloczyn wektorowy kierunku patrzenia i globalnego wektora Up.
    right_ = forward_.crossProduct(up).normalized();

    // Prawdziwy wektor "w górę" (oś Y kamery).
    trueUp_ = right_.crossProduct(forward_);

    // Konwersja kąta widzenia na radiany
    fovRadians_ = fovDegrees * (std::numbers::pi / 180.0);
}

Ray Camera::generateRay(int x, int y, int imageWidth, int imageHeight, double sampleX, double sampleY) const {
    
    // Sprowadza pozycję piksela (wraz z przesunięciem podpróbki dla antyaliasingu) do przedziału [0.0, 1.0].
    auto u = ( x + sampleX ) / static_cast<double>(imageWidth);
    auto v = ( y + sampleY ) / static_cast<double>(imageHeight);

    // Stosunek szerokości do wysokości obrazu. 
    auto aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);

    switch(type_) {
        case CameraType::PERSPECTIVE: {
            // Mapuje przedział [0, 1] na [-1, 1], koryguje proporcje (aspectRatio) i skaluje płaszczyznę
            // rzutowania tangensem połowy kąta widzenia. Symuluje to ogniskową (focal length) soczewki.
            auto px = ( 2.0 * u - 1.0) * aspectRatio * std::tan(fovRadians_ * 0.5);
            auto py = ( 1.0 - 2.0 * v) * std::tan(fovRadians_ * 0.5);

            auto direction = (forward_ + right_ * px + trueUp_ * py).normalized();
            return {eye_, direction};
        }
        case CameraType::ORTHOGRAPHIC: {
            // Rzutowanie równoległe 
            // Zamiast tangensa kąta FOV, do skalowania przestrzeni używamy sztywnego mnożnika viewScale_.
            auto screenX = ( 2.0 * u - 1.0) * aspectRatio * viewScale_;
            auto screenY = ( 1.0 - 2.0 * v) * viewScale_;

            // W modelu ortograficznym wszystkie promienie lecą idealnie równolegle (direction = forward_).
            // Zmienia się natomiast punkt startowy (origin) na płaszczyźnie tnącej kamery.
            auto origin = eye_ + right_ * screenX + trueUp_ * screenY;
            auto direction = forward_;

            return {origin, direction};
        }
        default:
            return {eye_, forward_};
    }
}