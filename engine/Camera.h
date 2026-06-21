#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "Ray.h"
#include "Vector.h"

// Typy rzutowania (projekcji) używane do mapowania przestrzeni 3D na płaszczyznę 2D.
enum class CameraType {
    // Rzutowanie równoległe (izometryczne). Brak efektu skracania perspektywicznego.
    // Promienie są zawsze równoległe do siebie. Zastosowanie: inżynieria, rzuty CAD, interfejsy 2D.
    ORTHOGRAPHIC,

    // Rzutowanie perspektywiczne. Symuluje ludzkie oko oraz fizyczne soczewki optyczne.
    // Promienie rozchodzą się promieniście z jednego punktu (focal point). Posiada głębię.
    PERSPECTIVE
};

// Klasa wirtualnej kamery (obserwatora).
// Generuje początkowe promienie optyczne (Primary Rays), które silnik wypuszcza w stronę sceny.
class Camera {
public:
    // Konstruktor budujący lokalny układ współrzędnych kamery (View Space / Camera Space).
    // Konwertuje ludzkie parametry (skąd patrzę, na co patrzę) na ścisłą bazę ortonormalną (ONB).
    Camera(CameraType type, Vector eye, Vector target, Vector up,
           double fovDegrees = 90.0, double viewScale = 1.0);

    // Najważniejsza metoda dla pętli renderującej (wywoływana dla każdego pod-piksela).
    // Mapuje współrzędne rastrowe (piksel X, Y) na wektor kierunkowy promienia w przestrzeni świata 3D.
    // Parametry sampleX i sampleY przyjmują przesunięcia z Samplera dla algorytmów antyaliasingu.
    Ray generateRay(int x, int y, int imageWidth, int imageHeight,
                    double sampleX, double sampleY) const;

private:
    CameraType type_;

    // Fizyczna pozycja soczewki w przestrzeni 3D (punkt startowy promieni perspektywicznych).
    Vector eye_;

    // Kąt widzenia (Field of View) przeliczony na radiany. Kontroluje, jak duży wycinek sceny
    // jest widoczny. Wpływa bezpośrednio na płaszczyznę rzutowania (symulacja ogniskowej).
    double fovRadians_;

    // Mnożnik skali dla rzutowania ortograficznego (ponieważ nie posiada ono FOV).
    double viewScale_;

    // --- Baza Ortonormalna (Orthonormal Basis - ONB) układu kamery ---
    // Trzy wzajemnie prostopadłe wektory o długości 1.0, definiujące orientację kamery w przestrzeni.

    // Oś Z kamery (kierunek patrzenia).
    Vector forward_;

    // Oś X kamery (wektor wychylenia w prawo).
    Vector right_;

    // Oś Y kamery (prawdziwy wektor pionu, skorygowany matematycznie względem forward_ i right_).
    Vector trueUp_;
};

#endif //RAYTRACING_CAMERA_H