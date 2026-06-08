#include "Renderer.h"
#include <execution> 
#include <numeric> 

void Renderer::render(const Scene& scene, const Camera& camera, Image& image,
                      const std::vector<Sample2D>& samples) const {
    
    auto width = image.getWidth();
    auto height = image.getHeight();
    
    // Inicjalizacja wektora przechowującego numery wierszy obrazu
    std::vector<int> rows(height);
    std::iota(rows.begin(), rows.end(), 0);
    
    // Zrównoleglenie obliczeń (wielowątkowość)
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
    
    // Algorytm antyaliasingu
    for (const Sample2D& sample : samples) {
        Ray ray = camera.generateRay(x, y, imageWidth, imageHeight, sample.x, sample.y);
        
        //depth = 5 ilosc odbic
        sum += trace(ray, scene, 10); 
    }
    
    return sum / static_cast<double>(samples.size());
}

Color Renderer::trace(const Ray& ray, const Scene& scene, int depth) const {
    IntersectionResult hit = scene.intersect(ray, 0.001, 1e9); 
    
    if (hit.type != MISS) {
        return shade(hit, scene, ray, depth);
    }
    
    return scene.backgroundColor;
}

Color Renderer::shade(const IntersectionResult& hit, const Scene& scene, const Ray& ray, int depth) const {
    Material mat = hit.material;
    Color finalColor(0.0, 0.0, 0.0);
    
    // ROZDZIELENIE OŚWIETLENIA:
    Color baseColor(0.0, 0.0, 0.0);     // Ambient + Diffuse (kolor gaśnie na szkle)
    Color specularColor(0.0, 0.0, 0.0);
    
    // --- 1. AMBIENT ---
    Color ambient = mat.baseColor * mat.Ka;
    baseColor += ambient;
    
    Vector N = hit.intersectionLPOINTNormal.normalized();
    Vector V = (ray.origin() - hit.LPOINT).normalized(); 
    
    // --- MODEL PHONGA I CIENIE ---
    for (const auto& light : scene.getLights()) {
        
        Vector L = light->getDirectionFrom(hit.LPOINT).normalized();
        double distanceToLight = light->getDistanceFrom(hit.LPOINT);
        Color lightIntensity = light->getIntensityAt(hit.LPOINT);
        
        // --- 2. CIENIE ---
        Ray shadowRay(hit.LPOINT + (N * 0.001), L);
        IntersectionResult shadowHit = scene.intersect(shadowRay, 0.001, distanceToLight);
        
        if (shadowHit.type != MISS) {
            continue; 
        }
        
        // --- 3. DIFFUSE ---
        double nDotL = std::max(0.0, N.dotProduct(L));
        Color diffuse = mat.baseColor * lightIntensity * (mat.Kd * nDotL);
        baseColor += diffuse; // Zapisujemy tylko do bazy!
        
        // --- 4. SPECULAR ---
        if (nDotL > 0.0) { 
            Vector R = (N * (2.0 * nDotL) - L).normalized();
            double rDotV = std::max(0.0, R.dotProduct(V));
            double specularFactor = std::pow(rDotV, mat.n);
            
            // Zapisujemy bezpośrednio do oddzielnego koszyka na rozbłyski!
            specularColor += lightIntensity * (mat.Ks * specularFactor);     
        }
    }
    
    // --- REKURENCJA: ODBICIA I ZAŁAMANIA ---
    Color reflectedColor(0.0, 0.0, 0.0);
    Color refractedColor(0.0, 0.0, 0.0);
    
    if (depth > 0) {
        Vector I = ray.direction().normalized(); 
        Vector N_norm = hit.intersectionLPOINTNormal.normalized();
       
        // --- LUSTRO ---
        if (mat.reflectivity > 0.0) {
            Vector R = (I - N_norm * (2.0 * I.dotProduct(N_norm))).normalized();
            Ray reflectedRay(hit.LPOINT + (N_norm * 0.001), R);
            reflectedColor = trace(reflectedRay, scene, depth - 1);
        }
        
        // --- SZKŁO ---
        if (mat.transparency > 0.0) {
            auto n1 = 1.0;         
            auto n2 = mat.ior;     
            auto n_refract = N_norm;     
           
            auto cosI = I.dotProduct(N_norm);
           
            if (cosI > 0.0) {
                std::swap(n1, n2);   
                n_refract = N_norm * -1.0; 
            } else {
                cosI = -cosI; 
            }
           
            auto eta = n1 / n2;
            auto k = 1.0 - eta * eta * (1.0 - cosI * cosI);
           
            if (k >= 0.0) { 
                auto T = (I * eta) + (n_refract * (eta * cosI - std::sqrt(k)));
                T.normalize();
                
                Ray refractedRay(hit.LPOINT - (n_refract * 0.001), T);
                refractedColor = trace(refractedRay, scene, depth - 1);
            }
        }
    }
    
    // --- 5. MIKSOWANIE KOLORÓW ---
    double localWeight = 1.0 - mat.reflectivity - mat.transparency;
    if (localWeight < 0.0) localWeight = 0.0;
    
    // baseColor mnożymy przez wagę (więc szkło nie ma własnego koloru),
    // ale specularColor DODAJEMY bezwarunkowo, żeby szkło miało ostry błysk z żarówki
    finalColor = (baseColor * localWeight) + 
                 specularColor + 
                 (reflectedColor * mat.reflectivity) + 
                 (refractedColor * mat.transparency);
                 
    // Ograniczenie barw do max 1.0
    finalColor.setRed(std::min(1.0, finalColor.red()));
    finalColor.setGreen(std::min(1.0, finalColor.green()));
    finalColor.setBlue(std::min(1.0, finalColor.blue()));
    
    return finalColor;
}