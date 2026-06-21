#include "Ray.h"
#include <iostream>

// Domyślny konstruktor promienia.
// Inicjalizuje bezpieczny wektor kierunkowy (1.0, 0.0, 0.0), co zapobiega pojawieniu się
// wektora zerowego. Wektory zerowe prowadzą do błędu dzielenia przez zero (NaN) w równaniach.
Ray::Ray() : Distance_ (0.0) {
    Direction_ = Vector(1.0, 0.0, 0.0);
}

// Konstruktor promienia oparty na wektorze łączącym dwa punkty.
// Idealny dla promieni cienia (Shadow Rays), gdzie 'direction' to wektor od uderzenia w obiekt
// DO źródła światła (L - O). Silnik automatycznie wydobywa z niego odległość (limit cienia).
Ray::Ray(Vector origin, Vector direction) {
    Origin_ = origin;
    Distance_ = direction.length();

    // Normalizacja wektora D (Direction) jest krytyczna dla całego silnika.
    // Dzięki temu, w równaniu parametrycznym promienia P(t) = O + tD, parametr 't'
    // jest dokładną odległością fizyczną (w metrach/jednostkach sceny).
    if (Distance_ != 0.0) {
        Direction_ = direction.normalized();
    } else {
        Direction_ = Vector(1.0, 0.0, 0.0);
    }

    // Wyliczenie punktu docelowego. Przydatne do testów Z-Buffera, aby sprawdzić
    // czy przeszkoda znajduje się bliżej niż wynosi zdefiniowany dystans (np. pozycja światła).
    Destination_ = Origin_ + (Direction_ * Distance_);
}

// Konstruktor pomocniczy/testowy.
// Rzuca promień równolegle do osi X na zadaną odległość.
Ray::Ray(Vector origin, double distance) {
    Origin_ = origin;
    Distance_ = distance;
    Direction_ = Vector(1.0, 0.0, 0.0);
    Destination_ = Origin_ + (Direction_ * Distance_);
}

// Główny konstruktor dla promieni kamery (Primary Rays) i promieni odbitych (Reflection/Refraction).
// Rozdziela fizyczny kierunek lotu fotonu od dystansu ucięcia (np. render distance).
Ray::Ray(Vector origin, Vector direction, double distance) {
    Origin_ = origin;
    Distance_ = distance;

    // Normalizacja i zabezpieczenie przed podaniem zdegenerowanego wektora wejściowego.
    if ( direction.length() != 0.0 ) {
        Direction_ = direction.normalized();
    } else {
        Direction_ = Vector(1.0, 0.0, 0.0);
    }

    Destination_ = Origin_ + (Direction_ * distance);
}

// Diagnostyczny zrzut stanu pamięci promienia do konsoli.
// Narzędzie do debugowania błędów matematycznych (np. gdy ekran nagle staje się czarny,
// można sprawdzić, czy współrzędne Origin lub Dir nie uległy uszkodzeniu do wartości NaN).
std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << "Origin: " << ray.Origin_
    << ", Dir: " << ray.Direction_
    << ", Distance: " << ray.Distance_
    << ", Dest: " << ray.Destination_;
    return os;
}