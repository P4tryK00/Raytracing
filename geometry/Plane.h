#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include <iosfwd>
#include "Vector.h"
#include "Ray.h"
#include "IntersectionResult.h"

// Reprezentacja nieskończonej płaszczyzny matematycznej w przestrzeni 3D.
// W silnikach ray tracingu płaszczyzny są używane jako optymalny sposób definiowania
// podłóg, ścian czy granic środowiska, ponieważ wymagają tylko jednego równania analitycznego
// zamiast dziesiątek trójkątów i nie obciążają pamięci RAM (bardzo niski koszt intersekcji).
// Klasa opiera się na algebraicznej postaci normalnej (Hessian normal form): N * P + d = 0.
class Plane {
public:
    // Domyślny konstruktor. Tworzy płaszczyznę przechodzącą przez centrum układu (0,0,0).
    Plane();

    // Inicjalizuje płaszczyznę na podstawie wektora normalnego (N) oraz odległości skalarnej (d).
    // Odległość to minimalny dystans od początku układu współrzędnych do tej płaszczyzny.
    Plane(Vector normal, double distance);

    // Pełny konstruktor przypisujący płaszczyźnie konkretne właściwości fizyczne (szkło, lustro, mat itp.).
    Plane(Vector normal, double distance, Material mat);

    [[nodiscard]] Vector Normal() const {return normal_;}
    void Normal(const Vector& n) { normal_ = n;}

    [[nodiscard]] double Distance() const { return distance_;}
    void Distance(double d) { distance_ = d; }

    // Narzędzie analityczne do obliczania punktu przecięcia trzech dowolnych płaszczyzn.
    // Implementuje rozwiązanie układu trzech równań liniowych przy użyciu Prawa Cramera
    // oraz iloczynu mieszanego. Zwraca 'true', jeśli płaszczyzny faktycznie zbiegają się w jednym punkcie.
    bool Intersect3(const Plane &p2, const Plane &p3, Vector &result) const;

    // Główna funkcja testująca zderzenie promienia (Ray) z powierzchnią.
    // Podstawia parametryczne równanie promienia P(t) do równania płaszczyzny w celu znalezienia
    // skalara 't' (dystansu). Jeśli 't' mieści się w limitach z-buffera (t_min, t_max), zgłasza HIT.
    [[nodiscard]] IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;

    // Przeciążenie strumienia używane do zrzutu stanu wektorów do konsoli podczas debugowania logiki przestrzennej.
    friend std::ostream& operator<<(std::ostream& os, const Plane& p);

private:
    // N: Znormalizowany wektor prostopadły do powierzchni. Definiuje orientację płaszczyzny.
    Vector normal_;

    // d: Przesunięcie płaszczyzny wzdłuż jej wektora normalnego względem zera (Origin).
    double distance_;

    // Zestaw współczynników modelu oświetlenia używany przez Renderer (Kd, Ks, ior, itd.).
    Material material_;
};

#endif //RAYTRACING_PLANE_H