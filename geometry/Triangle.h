#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H

#include "Vector.h"
#include "Ray.h"
#include "IntersectionResult.h"

// Reprezentacja trójkąta w przestrzeni 3D.
// Podstawowy, najniższy poziom budulcowy dla złożonych modeli trójwymiarowych (Siatki / Meshes).
// Ze względu na to, że płaskość trójkąta jest matematycznie gwarantowana (przez dokładnie 3 punkty),
// jest to idealny prymityw geometryczny do budowania dowolnych obiektów na scenie.
class Triangle {
public:
    // Konstruktor trójkąta. Wierzchołki powinny być podawane w kolejności
    // przeciwnej do ruchu wskazówek zegara (CCW), by prawidłowo wyznaczyć wektor normalny.
    // Podczas inicjalizacji od razu pre-kalkuluje i buforuje normalną powierzchni (Flat Shading).
    Triangle(Vector v0, Vector v1, Vector v2, Material mat);

    // Funkcja testująca intersekcję fotonu (promienia) z obszarem trójkąta.
    // Zaimplementowana w oparciu o super-wydajny algorytm Möllera-Trumbore'a.
    // Metoda wyznacza współrzędne barycentryczne (u, v), co drastycznie redukuje ilość
    // potrzebnych operacji matematycznych względem klasycznego testu "płaszczyzna -> wielokąt".
    [[nodiscard]] IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;

    // Zwraca zbuforowany wektor prostopadły do całej powierzchni trójkąta.
    [[nodiscard]] Vector Normal() const { return normal_;}

private:
    // Pamięć geometrii: 3 wierzchołki w globalnym układzie współrzędnych (World Space).
    Vector v0_, v1_, v2_;

    // Zbuforowana normalna. Ponieważ trójkąt jest płaski, wektor ten nigdy się nie zmienia.
    // Pamiętanie go tutaj omija konieczność robienia kosztownego iloczynu wektorowego
    // przez każdy pojedynczy promień uderzający w ten obiekt.
    Vector normal_;

    // Struktura parametrów optycznych przekazywana do silnika po potwierdzeniu kolizji.
    Material material_;
};

#endif //RAYTRACING_TRIANGLE_H