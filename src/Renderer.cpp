#include "Renderer.h"
#include <execution> 
#include <numeric> 


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
        return shade(hit, scene, ray);
    }
    
    return Color(0.1, 0.1, 0.1);
}

Color Renderer::shade(const IntersectionResult& hit, const Scene& scene, const Ray& ray) const {
    Material mat = hit.material;
    Color finalColor(0.0, 0.0, 0.0);
    
    // 1. AMBIENT: Zawsze obecne światło rozproszone. 
    // Symuluje oświetlenie globalne.
    Color ambient = mat.baseColor * mat.Ka;
    finalColor += ambient;
    
    // Normalna (N) w punkcie przecięcia
    Vector N = hit.intersectionLPOINTNormal.normalized();
    
    // Wektor kierunku do obserwatora / kamery (V) - odwrócony wektor promienia
    Vector V = (ray.origin() - hit.LPOINT).normalized(); 
    
    // Iterujemy przez wszystkie światła na scenie
    for (const auto& light : scene.getLights()) {
        
        // Wektor kierunkowy do światła (L) oraz odległość
        Vector L = light->getDirectionFrom(hit.LPOINT).normalized();
        double distanceToLight = light->getDistanceFrom(hit.LPOINT);
        Color lightIntensity = light->getIntensityAt(hit.LPOINT);
        
        // --- 2. CIENIE (Shadow Ray) ---
        // Generujemy promień z punktu przecięcia w stronę światła.
        // Używamy "epsilon" (hit.LPOINT + N * 0.001), aby przesunąć punkt minimalnie
        // nad powierzchnię figury i uniknąć "trądziku cieni" (samoprzesłaniania).
        Ray shadowRay(hit.LPOINT + (N * 0.001), L);
        
        // Szukamy przecięć od 0.001 tylko do odległości źródła światła (nie dalej!)
        IntersectionResult shadowHit = scene.intersect(shadowRay, 0.001, distanceToLight);
        
        if (shadowHit.type != MISS) {
            // Coś stoi na drodze do światła!
            // Ten punkt obiektu jest w cieniu (zostaje na nim tylko światło ambient).
            continue; 
        }
        
        // --- 3. DIFFUSE (Model Lamberta) ---
        // Obliczamy iloczyn skalarny normalnej i wektora do światła (L * N)
        // std::max ucina wartości ujemne (gdy światło oświetla obiekt z tyłu)
        double nDotL = std::max(0.0, N.dotProduct(L));
        Color diffuse = mat.baseColor * lightIntensity * (mat.Kd * nDotL);
        
        // --- 4. SPECULAR (Rozbłysk Phonga) ---
        Color specular(0.0, 0.0, 0.0);
        if (nDotL > 0.0) { // Liczymy rozbłysk tylko, jeśli światło dociera do tej strony obiektu
            // R = 2 * (N * L) * N - L
            Vector R = (N * (2.0 * nDotL) - L).normalized();
            
            double rDotV = std::max(0.0, R.dotProduct(V));
            
            // Podnosimy do potęgi 'n' (Shininess)
            double specularFactor = std::pow(rDotV, mat.n);
            
            // Specular zazwyczaj przybiera kolor światła (np. biały rozbłysk), a nie obiektu
            specular = lightIntensity * (mat.Ks * specularFactor);
        }
        
        // Sumujemy ostateczny kolor
        finalColor += (diffuse + specular);
    }
    
    
    finalColor.setRed(std::min(1.0, finalColor.red()));
    finalColor.setGreen(std::min(1.0, finalColor.green()));
    finalColor.setBlue(std::min(1.0, finalColor.blue()));
    
    return finalColor;
}
