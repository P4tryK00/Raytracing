#ifndef VECTOR_RAY_H
#define VECTOR_RAY_H

#include <iosfwd>

#include "Vector.h"

// Klasa reprezentująca pojedynczy promień optyczny (foton) propagujący w przestrzeni 3D.
// Bazuje na równaniu parametrycznym P(t) = O + tD. Wykorzystywana do strzelania promieniami
// z kamery (Primary Rays), badania oświetlenia (Shadow Rays) oraz obliczania załamań/odbić.
class Ray {
public:
    // Inicjalizuje zdegenerowany promień z dystansem 0.0 i wektorem kierunku wzdłuż osi X.
    Ray();

    // Inicjalizuje promień z nienormalizowanego wektora docelowego (np. wektor do źródła światła).
    // Silnik automatycznie go znormalizuje i wyciągnie z niego limit odległości propagacji.
    Ray(Vector origin, Vector direction);

    // Inicjalizuje promień testowy wzdłuż osi X o zadanej odległości propagacji.
    Ray(Vector origin, double distance);

    // Główny konstruktor dla optyki geometrycznej. Przyjmuje oddzielnie wektor lotu i limit dystansu.
    // Wymusza normalizację wektora i od razu pre-kalkuluje punkt docelowy.
    Ray(Vector origin, Vector direction, double distance);

    Vector origin() const { return Origin_; }
    void origin(Vector o) { Origin_ = o; }

    Vector destination() const { return Destination_; }
    void destination(Vector d) { Destination_ = d;}

    Vector direction() const { return Direction_; }
    void direction(Vector d) { Direction_ = d; }

    double distance() const { return Distance_; }
    void distance(double d) { Distance_ = d; }

    // Wsparcie zrzutu stanu obiektu dla konsoli w celach diagnostycznych (debugowanie intersekcji).
    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);

private:
    // 'O' (Origin): Punkt początkowy promienia.
    // Reprezentuje miejsce powstania promienia (np. pozycja soczewki kamery lub punkt uderzenia w obiekt).
    Vector Origin_;

    // Wyliczony ostateczny punkt uderzenia (O + t*D) przy maksymalnym dystansie.
    // Stosowany do optymalizacji testów Z-Buffera i cieni (aby nie sprawdzać za daleko).
    Vector Destination_;

    // 'D' (Direction): Znormalizowany wektor promienisty określający tor lotu.
    // Krytyczne jest utrzymanie długości równej 1.0, w przeciwnym razie algorytmy przecięcia zwrócą błędne dane.
    Vector Direction_;

    // 't' (Distance): Aktualna odległość / limit propagacji promienia.
    // Skraca tor lotu promienia, pozwalając na wczesne odrzucanie kolizji (np. cieni za źródłem światła).
    double Distance_;
};

#endif //VECTOR_RAY_H