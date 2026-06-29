 #include "Triangle.h"

#include <cmath>

// Konstruktor trójkąta
// w porządku przeciwnym do ruchu wskazówek zegara 
Triangle::Triangle(Vector v0, Vector v1, Vector v2, Material mat) : v0_(v0), v1_(v1), v2_(v2), material_(mat) {

    // Obliczenie dwóch wektorów krawędzi wychodzących z pierwszego wierzchołka.
    Vector edge1 = v1 - v0;
    Vector edge2 = v2 - v0;

    // Wektor normalny powierzchni
    // Iloczyn wektorowy dwóch krawędzi daje wektor idealnie prostopadły do powierzchni trójkąta.
    normal_ = edge1.crossProduct(edge2).normalized();
}

// algorytm przecięcia promienia z trójkątem 
// Przekształca problem z przestrzeni trójwymiarowej (x, y, z) na dwuwymiarowe współrzędne
// barycentryczne (u, v) rozpięte na krawędziach samego trójkąta.
IntersectionResult Triangle::intersect(const Ray& ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;

    Vector edge1 = v1_ - v0_;
    Vector edge2 = v2_ - v0_;

    // Iloczyn wektorowy kierunku promienia i drugiej krawędzi.
    Vector h = ray.direction().crossProduct(edge2);

    // Wyznacznik macierzy transformacji 
    auto a = edge1.dotProduct(h);

    constexpr auto EPSILON = 1e-8;

    // Jeśli wyznacznik jest bliski zeru, promień i trójkąt są do siebie równoległe.
    // Oznacza to, że promień "ślizga się" po płaszczyźnie trójkąta lub ją całkowicie mija.
    if (std::abs(a) < EPSILON) {
        return result;
    }

    // Odwrotność wyznacznika
    auto f = 1.0 / a;

    // Wektor przesunięcia od wierzchołka v0 do początku promienia (Origin).
    Vector s = ray.origin() - v0_;

    // Obliczenie pierwszej współrzędnej barycentrycznej 'u'.
    auto u = f * s.dotProduct(h);

    // Test przekroczenia granic: jeśli 'u' jest ujemne lub większe niż 1.0,
    // promień przebija płaszczyznę POZA obszarem naszego trójkąta.
    // Używamy Epsilona przy testach, by zapobiec tzw. zjawisku "pękania" siatki
    // (dziurom na styku dwóch trójkątów) wywołanym błędem precyzji zmiennoprzecinkowej.
    if (u < -EPSILON || u > 1.0 + EPSILON ) {
        return result;
    }

    Vector q = s.crossProduct(edge1);

    // Obliczenie drugiej współrzędnej barycentrycznej 'v'.
    auto v = f * ray.direction().dotProduct(q);

    // Test granic dla 'v'. W systemie barycentrycznym suma u + v nie może przekroczyć 1.0.
    // Jeśli przekracza, punkt leży w prawym-górnym rogu równoległoboku rozpiętego przez
    // edge1 i edge2, ale już poza przekątną odcinającą nasz trójkąt.
    if ( v < -EPSILON || v + u > 1.0 + EPSILON) {
        return result;
    }

    // Na tym etapie jesteśmy matematycznie pewni, że uderzyliśmy WNĘTRZE trójkąta.
    // Obliczamy parametr 't' z równania promienia, by określić fizyczną odległość uderzenia.
    auto t = f * edge2.dotProduct(q);

    // Test czy uderzenie jest bliżej niż aktualnie widoczny obiekt i nie leży za kamerą?
    if (t > t_min && t < t_max) {

        // test strony uderzenia.
        // Jeśli promień leci w kierunku przeciwnym do normalnej, uderza w przód siatki.
        if ( normal_.dotProduct(ray.direction()) < -EPSILON) {
            result.type = HIT;
        } else {
            // W przeciwnym razie uderzył od środka
            result.type = INSIDE_PRIMITIVE;
        }

        result.distance = t;
        result.LPOINT = ray.origin() + ray.direction() * t;

        // Trójkąt płaski (Flat Shaded) współdzieli ten sam wektor normalny na całej powierzchni.
        result.intersectionLPOINTNormal = normal_;

        result.material = material_;
    }

    return result;
}