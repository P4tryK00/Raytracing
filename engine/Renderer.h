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
    // Iteruje po wszystkich pikselach matrycy (z wykorzystaniem wielowątkowości w .cpp) 
    void render(const Scene& scene, const Camera& camera, Image& image, const std::vector<Sample2D>& samples) const;

private:
    // Oblicza ostateczny kolor pojedynczego piksela.
    // Realizuje numeryczne całkowanie światła (Monte Carlo Integration) poprzez wypuszczenie
    // pakietu promieni (samples) i uśrednienie ich wyników. Nakłada również Tone Mapping (ekspozycję).
    Color renderPixel(int x, int y, const Scene& scene, const Camera& camera,
                      int imageWidth, int imageHeight, const std::vector<Sample2D>& samples) const;

    // Rekurencyjna funkcja badająca tor lotu pojedynczego promienia (fotou).
    // Odpytuje scenę o punkt kolizji. Parametr 'depth' to twardy limit zagnieżdżeń (zabezpieczenie
    // przed Stack Overflow w przypadku promienia uwięzionego między dwoma równoległymi lustrami).
    Color trace(const Ray& ray, const Scene& scene, int depth) const;

    
    // Oblicza rówanie renderowania dla konkretnego punktu uderzenia (IntersectionResult).
    // Łączy oświetlenie lokalne (cienie, matowe rozproszenie Lamberta, lustrzany rozbłysk Phonga)
    // z oświetleniem globalnym (generowanie nowych promieni odbitych i załamanych z użyciem Prawa Snella).
    Color shade(const IntersectionResult& hit, const Scene& scene, const Ray& ray, int depth) const;

};

#endif //RAYTRACING_RENDERER_H