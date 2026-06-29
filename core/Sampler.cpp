#include "Sampler.h"
#include <random>


std::vector<Sample2D> Sampler::makeCenterSample() {
    return { {0.5, 0.5} };
}

// Dzieli obszar piksela na n*n równych kwadratów i wypuszcza promień dokładnie przez środek każdego z nich.
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

// Dzieli piksel na regularną siatkę n*n komórek  ale wewnątrz każdej komórki losuje przesunięcie promienia.
std::vector<Sample2D> Sampler::makeJitteredSamples(int n) {
    std::vector<Sample2D> samples;
    samples.reserve(n * n);

    std::random_device rd;
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