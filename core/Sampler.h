#ifndef RAYTRACING_SAMPLER_H
#define RAYTRACING_SAMPLER_H
#include <vector>

// Struktura reprezentująca pojedynczą podpróbkę (sub-pixel sample) w dwuwymiarowej przestrzeni ekranu.
// Wartości 'x' i 'y' są znormalizowane do przedziału [0.0, 1.0],
// gdzie (0.0, 0.0) to lewy górny róg, a (1.0, 1.0) to prawy dolny róg konkretnego piksela.
struct Sample2D{
    double x;
    double y;
};

// Statyczna klasa narzędziowa generująca wzorce próbkowania (Sampling Patterns).
// Odpowiada za dostarczanie współrzędnych startowych dla promieni rzucanych przez kamerę.
// Jest to matematyczna podstawa technik antyaliasingu (wygładzania krawędzi) w ray tracingu.
class Sampler {
public:
    // Generuje najprostszą, pojedynczą próbkę uderzającą w fizyczny środek piksela (0.5, 0.5).
    // Koszt: 1 promień/piksel. Brak wygładzania (skutkuje widocznymi "schodkami" na krawędziach).
    // Zastosowanie: Bardzo szybki render podglądowy geometrii sceny.
    static std::vector<Sample2D> makeCenterSample();

    // Generuje sztywną, matematycznie równomierną siatkę podpróbek o wymiarach n*n (Supersampling).
    // Koszt: n^2 promieni/piksel.
    // Choć dobrze wygładza proste linie, ze względu na przewidywalność równomiernej siatki
    // algorytm ten jest podatny na fatalne w skutkach artefakty strukturalne (np. efekt mory).
    static std::vector<Sample2D> makeRegularSample(int n);

    // Generuje równomierne próbkowanie z nałożonym szumem (Jittered / Stratified Sampling).
    // Dzieli piksel na komórki n*n, ale pozycję fotonu wewnątrz komórki ustala losowo.
    // Złoty standard klasycznego Monte Carlo - eliminuje efekt mory zamieniając go
    // na wysokoczęstotliwościowy szum obrazu, który ludzkie oko łatwo toleruje.
    static std::vector<Sample2D> makeJitteredSamples(int n);
};

#endif //RAYTRACING_SAMPLER_H