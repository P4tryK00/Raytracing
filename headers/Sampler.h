#ifndef RAYTRACING_SAMPLER_H
#define RAYTRACING_SAMPLER_H
#include <vector>


struct Sample2D{
    double x;
    double y;
};

class Sampler {
public:
    static std::vector<Sample2D> makeCenterSample();
    static std::vector<Sample2D> makeRegularSample(int n);
    static std::vector<Sample2D> makeJitteredSamples(int n);
};


#endif //RAYTRACING_SAMPLER_H
