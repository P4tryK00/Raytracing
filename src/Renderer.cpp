#include "Renderer.h"
#include <execution> //test 
#include <numeric> // test

void Renderer::render(const Scene& scene, const Camera& camera, Image& image,
    const std::vector<Sample2D>& samples) const {
    
    auto width = image.getWidth();
    auto height = image.getHeight();
    
    std::vector<int> rows(height);
    std::iota(rows.begin(), rows.end(), 0);
    
    std::for_each(std::execution::par, rows.begin(), rows.end(), [&](int y) {
        for (int x = 0; x < width; ++x) {
            Color pixelColor = renderPixel(x, y, scene, camera, width, height, samples);
            image.setPixel(x, y, pixelColor);
        }
        
    });
    
}

Color Renderer::renderPixel(int x, int y, const Scene& scene, const Camera& camera,
    int imageWidth,int imageHeight, const std::vector<Sample2D>& samples) const {
    Color sum(0.0, 0.0, 0.0);
    for (const Sample2D& sample : samples) {
        Ray ray = camera.generateRay(x, y, imageWidth, imageHeight, sample.x, sample.y);
        sum += trace(ray, scene);
    }
    return sum / static_cast<double>(samples.size());
}

Color Renderer::trace(const Ray& ray, const Scene& scene) const {
    IntersectionResult hit = scene.intersect(ray, 0.001, 1e9); 
    if (hit.type != MISS) {
        return shadeAmbient(hit); 
    }
    return scene.backgroundColor;
}

Color Renderer::shade(const IntersectionResult& hit) const {
    return hit.material.baseColor;
}

Color Renderer::shadeAmbient(const IntersectionResult& hit) const {
    Color ambient(0.2, 0.2, 0.2);
    return hit.material.baseColor * ambient; 
}