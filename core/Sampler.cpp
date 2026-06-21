#include "Sampler.h"
#include <random>

// Generator podstawowej próbki na środku piksela.
// Używany do bardzo szybkiego renderowania podglądowego bez antyaliasingu (1 promień na piksel).
// Skutkuje twardymi, ząbkowanymi krawędziami geometrii (tzw. jaggies).
std::vector<Sample2D> Sampler::makeCenterSample() {
    return { {0.5, 0.5} };
}

// Generator równomiernej siatki podpróbek (Regular Grid Supersampling).
// Dzieli obszar piksela na n*n równych kwadratów i wypuszcza promień dokładnie przez środek każdego z nich.
// Dobrze wygładza proste krawędzie geometrii, ale ze względu na swoją stałą regularność
// algorytm ten jest podatny na artefakty (np. zjawisko mory - Moiré pattern) przy bardzo gęstych strukturach.
std::vector<Sample2D> Sampler::makeRegularSample(int n) {
    std::vector<Sample2D> samples;
    samples.reserve(n*n);
    for ( auto py = 0; py < n; ++py ) {
        for ( auto px = 0; px < n; ++px ) {
            auto sampleX = (px + 0.5) / static_cast<double>(n);
            auto sampleY = (py + 0.5) / static_cast<double>(n);

            samples.push_back(Sample2D(sampleX, sampleY));
        }
    }
    return samples;
}

// Generator próbek warstwowych z szumem (Jittered / Stratified Sampling).
// Najważniejsza metoda antyaliasingu w klasycznym ray tracingu opartym na Monte Carlo.
// Dzieli piksel na regularną siatkę n*n komórek (by zachować równomierne pokrycie piksela),
// ale wewnątrz każdej komórki losuje przesunięcie promienia.
// Zamienia to regularne artefakty graficzne na wysokoczęstotliwościowy szum (noise),
// który jest znacznie lepiej tolerowany i uśredniany przez ludzkie oko.
std::vector<Sample2D> Sampler::makeJitteredSamples(int n) {
    std::vector<Sample2D> samples;
    samples.reserve(n * n);

    std::random_device rd;
    // Inicjalizacja generatora Mersenne Twister - standardu dla wydajnego losowania w silnikach 3D
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (int py = 0; py < n; ++py) {
        for (int px = 0; px < n; ++px) {
            // Dodanie losowego wektora przesunięcia 'dis(gen)' wewnątrz przypisanej komórki (px, py)
            double sampleX = (px + dis(gen)) / static_cast<double>(n);
            double sampleY = (py + dis(gen)) / static_cast<double>(n);

            samples.push_back(Sample2D{sampleX, sampleY});
        }
    }
    return samples;
}