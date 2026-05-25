#include "Sampler.h"
#include <random>

std::vector<Sample2D> Sampler::makeCenterSample() {
    return { {0.5, 0.5} };
}

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

std::vector<Sample2D> Sampler::makeJitteredSamples(int n) {
    std::vector<Sample2D> samples;
    samples.reserve(n * n);

    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<double> dis(0.0, 1.0); 

    for (int py = 0; py < n; ++py) {
        for (int px = 0; px < n; ++px) {
            double sampleX = (px + dis(gen)) / static_cast<double>(n);
            double sampleY = (py + dis(gen)) / static_cast<double>(n);
            
            samples.push_back(Sample2D{sampleX, sampleY});
        }
    }
    return samples;
}
