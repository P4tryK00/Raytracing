#ifndef RAYTRACING_RENDERER_H
#define RAYTRACING_RENDERER_H

#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Sampler.h"
#include "Color.h"
#include "Ray.h"
#include "IntersectionResult.h"
#include <vector>

class Renderer {
public:
    void render(const Scene& scene, const Camera& camera, Image& image, const std::vector<Sample2D>& samples) const;

private:
    Color renderPixel(int x, int y, const Scene& scene, const Camera& camera, int imageWidth, int imageHeight, const std::vector<Sample2D>& samples) const;
    Color trace(const Ray& ray, const Scene& scene) const;
    
    Color shade(const IntersectionResult& hit, const Scene& scene, const Ray& ray) const;

};


#endif //RAYTRACING_RENDERER_H
