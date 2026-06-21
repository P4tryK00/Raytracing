#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <iosfwd>

// Fundament matematyczny całego silnika ray tracingu.
// Reprezentuje zarówno punkty w przestrzeni 3D (x, y, z), jak i wektory kierunkowe
// (np. tor lotu promienia, wektor normalny powierzchni geometrycznej).
class Vector {
    public:
    // Otwarte pola składowe ułatwiają bezpośredni i szybki dostęp do koordynat
    // podczas setek tysięcy obliczeń w pętli renderującej.
    double x,y,z;

    // --- KONSTRUKTORY ---

    // Tworzy wektor zerowy [0, 0, 0]. Używany często jako środek układu współrzędnych (Origin).
    Vector();

    // Bezpośrednia inicjalizacja składowych przestrzennych.
    Vector(double x, double y, double z);

    // Tworzy wektor kierunkowy łączący dwa punkty (celujący od v1 do v2).
    // Niezbędny do wyznaczania promieni (np. od punktu uderzenia w stronę żarówki).
    Vector(const Vector& v1, const Vector& v2);

    // --- PRZECIĄŻONE OPERATORY ALGEBRAICZNE ---
    void operator+=(const Vector& v);
    void operator-=(const Vector& v);
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;

    // Skalowanie wektora (zmiana długości bez zmiany zwrotu/kierunku).
    Vector operator*(double m) const;
    Vector operator/(double d) const;

    // --- OPERACJE MODYFIKUJĄCE (In-place) ---
    void divide(double d);
    void scalarMultiply(double m);

    // Skraca wektor do długości dokładnie 1.0 zachowując jego zwrot.
    // Krok krytyczny dla wektorów normalnych (N) i kierunków światła (L),
    // by iloczyn skalarny w modelu Phonga zwracał precyzyjny cosinus kąta padania.
    void normalize();

    // --- OPERACJE ZWRACAJĄCE NOWĄ WARTOŚĆ ---

    // Zwraca euklidesową długość wektora (np. wyliczanie fizycznego dystansu w przestrzeni).
    [[nodiscard]] double length() const;

    // Zwraca znormalizowaną kopię wektora, nie modyfikując obiektu bazowego.
    [[nodiscard]] Vector normalized() const;

    // --- KRYTYCZNE OPERACJE ALGEBRY LINIOWEJ ---

    // Iloczyn skalarny (Dot Product).
    // Najważniejsza operacja matematyczna w grafice 3D. Zwraca skalar proporcjonalny do
    // cosinusa kąta między znormalizowanymi wektorami. Służy do cieniowania matowego
    // (Prawo Lamberta), obliczania rozbłysków (Phong) oraz testów Backface Culling.
    [[nodiscard]] double dotProduct(const Vector& v) const;

    // Iloczyn wektorowy (Cross Product).
    // Generuje nowy wektor idealnie prostopadły do obu wektorów wejściowych.
    // Niezbędny do wyznaczania wektorów normalnych dla trójkątów i budowania
    // Bazy Ortonormalnej (ONB) dla wirtualnej kamery.
    [[nodiscard]] Vector crossProduct(const Vector& v) const;
};

// Globalny operator zapewniający przemienność mnożenia (skalar * wektor).
// Pozwala programiście pisać naturalnie wyglądające równania fizyczne (np. '2.0 * v')
// zamiast wymuszać odwrotny zapis obiektowy.
inline Vector operator*(double m, const Vector& v) {
    return v * m;
}

// Przeciążenie strumienia wyjściowego używane do zrzutu koordynatów w konsoli (diagnostyka).
std::ostream& operator<<(std::ostream& os, const Vector& v);

#endif //VECTOR_VECTOR_H