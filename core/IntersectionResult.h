#ifndef RAYTRACING_INTERSECTIONRESULT_H
#define RAYTRACING_INTERSECTIONRESULT_H
#include "Vector.h"
#include "Color.h"

// Stan promienia po teście intersekcji z figurą geometryczną.
// Flaga INSIDE_PRIMITIVE jest krytyczna dla materiałów przezroczystych (szkła) -
// informuje silnik optyczny, że promień aktualnie porusza się wewnątrz gęstszego ośrodka
// i przy wyjściu wektor normalny musi zostać odwrócony do obliczeń Prawa Snella.
enum IntersectionType {
    MISS,
    HIT,
    INSIDE_PRIMITIVE
};

// Struktura definiująca właściwości optyczne i fizyczne powierzchni obiektu.
// Opiera się na klasycznym, empirycznym modelu oświetlenia Phonga oraz
// rozszerzeniach dla rekursywnego śledzenia promieni (model Whitteda).
struct Material {
    // Albedo - fizyczny, natywny kolor materiału bez wpływu oświetlenia.
    Color baseColor = Color(1.0, 1.0, 1.0);

    // --- Współczynniki modelu Phonga (Oświetlenie Lokalne) ---
    // Ambient: Stała wartość symulująca globalne rozproszenie fotonów w pomieszczeniu.
    double Ka = 0.2;
    // Diffuse: Rozproszenie matowe (zgodne z prawem Lamberta). Zależy od iloczynu skalarnego światła i normalnej.
    double Kd = 0.8;
    // Specular: Bezpośrednie, kierunkowe odbicie źródła światła generujące biały "rozbłysk".
    double Ks = 0.5;
    // Shininess: Wykładnik potęgowy określający gładkość powierzchni.
    // Skupia rozbłysk światła (im większy, tym mniejsza i ostrzejsza plamka na obiekcie).
    double n  = 50.0;

    // --- Współczynniki Ray Tracingu (Oświetlenie Globalne) ---
    // Waga promienia odbitego. Określa, jak bardzo obiekt zachowuje się jak idealne lustro.
    double reflectivity = 0.0;

    // Waga promienia załamanego. Określa stopień przepuszczalności światła (dielektryki).
    double transparency = 0.0;
    // Index of Refraction (IOR). Fizyczny współczynnik gęstości optycznej ośrodka.
    // Decyduje o kącie załamania światła na granicy ośrodków (np. powietrze -> szkło).
    double ior = 1.0;

    Material() = default;
    Material(const Color& color, double ka, double kd, double ks, double shininess,
        double refl, double trans,double ior_val)
        : baseColor(color), Ka(ka), Kd(kd), Ks(ks), n(shininess),reflectivity(refl), transparency(trans), ior(ior_val)  {}
};

// Zunifikowany pakiet danych zwracany przez każdy test intersekcji (np. Sphere::intersect).
// Dostarcza silnikowi cieniującemu (Renderer::shade) absolutnie wszystkie informacje
// potrzebne do wyliczenia ostatecznego koloru punktu na scenie.
struct IntersectionResult {
    IntersectionType type = MISS;

    // 't' z równania promienia P = O + t*D. Używane do testów głębokości (Z-Buffer logic).
    double distance = 0.0;

    // Dokładne współrzędne 3D punktu trafienia promienia w obiekt.
    Vector LPOINT;

    // Znormalizowany wektor prostopadły do powierzchni w punkcie trafienia.
    // Najważniejszy wektor w grafice 3D - od niego zależą kąty odbicia, załamania i cienie.
    Vector intersectionLPOINTNormal;

    // Skopiowane właściwości fizyczne trafionego obiektu.
    Material material;
};

#endif //RAYTRACING_INTERSECTIONRESULT_H