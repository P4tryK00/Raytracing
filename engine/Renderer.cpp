#include "Renderer.h"
#include <execution> 
#include <numeric> 

// Główna pętla renderująca silnika.
// Rozdziela pracę na wszystkie dostępne rdzenie procesora (Multithreading) wykorzystując politykę std::execution::par.
// Renderowanie odbywa się wierszami (horyzontalnie), co jest optymalne dla pamięci cache (Cache Locality).
void Renderer::render(const Scene& scene, const Camera& camera, Image& image,
                      const std::vector<Sample2D>& samples) const {

    auto width = image.getWidth();
    auto height = image.getHeight();

    // Inicjalizacja wektora przechowującego indeksy wierszy obrazu (Y)
    std::vector<int> rows(height);
    std::iota(rows.begin(), rows.end(), 0);

    // Zrównoleglona pętla po wierszach obrazu
    std::for_each(std::execution::par, rows.begin(), rows.end(), [&](int y) {
        for (int x = 0; x < width; ++x) {
            Color pixelColor = renderPixel(x, y, scene, camera, width, height, samples);
            image.setPixel(x, y, pixelColor);
        }
    });
}

// Oblicza ostateczny kolor pojedynczego piksela na matrycy.
// Wykorzystuje numeryczne całkowanie Monte Carlo do uśrednienia wielu promieni (próbek) w obrębie jednego piksela,
// co skutkuje wysokiej jakości antyaliasingiem przestrzennym.
Color Renderer::renderPixel(int x, int y, const Scene& scene, const Camera& camera,
    int imageWidth,int imageHeight, const std::vector<Sample2D>& samples) const {

    Color sum(0.0, 0.0, 0.0);

    // Wypuszczanie pakietu promieni 
    for (const Sample2D& sample : samples) {
        Ray ray = camera.generateRay(x, y, imageWidth, imageHeight, sample.x, sample.y);

        // Rozpoczęcie rekurencji optycznej. Limit głębokości = 5 (zapobiega nieskończonym odbiciom w lustrach).
        sum += trace(ray, scene, 10);
    }

    // Uśrednienie zakumulowanego światła 
    Color finalColor = sum / static_cast<double>(samples.size());
    
    //  kompresja wysokich wartości natężenia światła (np. rozbłysków > 1.0) do przedziału [0.0, 1.0].
    // Zapobiega "przepaleniu" obrazu 
    double exposure = 1.0;
    double r = 1.0 - std::exp(-finalColor.red() * exposure);
    double g = 1.0 - std::exp(-finalColor.green() * exposure);
    double b = 1.0 - std::exp(-finalColor.blue() * exposure);
    
    return Color(r, g, b);
}

// Rekurencyjna funkcja śledzenia promienia 
// Bada kolizję promienia z obiektami w scenie i decyduje o przerwaniu lub kontynuacji 
Color Renderer::trace(const Ray& ray, const Scene& scene, int depth) const {
    // 0.001 to 'Epsilon' z-buffera, a 1e9 to limit renderowania
    IntersectionResult hit = scene.intersect(ray, 0.001, 1e9);

    if (hit.type != MISS) {
        // Punkt uderzenia istnieje - przekazanie sterowania do modelu oświetlenia
        return shade(hit, scene, ray, depth);
    }

    // Jeśli promień uciekł w pustkę, zwraca globalny kolor otoczenia 
    return scene.backgroundColor;
}

// Silnik cieniowania. Integruje oświetlenie lokalne (Phong, Lambert) z oświetleniem globalnym (odbicia, załamania).
Color Renderer::shade(const IntersectionResult& hit, const Scene& scene, const Ray& ray, int depth) const {
    Material mat = hit.material;
    Color finalColor(0.0, 0.0, 0.0);

    // Fizyczna separacja światła bazowego od rozbłysków lustrzanych.
    // Dzięki temu szkło może tracić swój bazowy kolor, ale wciąż "łapać" ostre refleksy od żarówek.
    Color baseColor(0.0, 0.0, 0.0);
    Color specularColor(0.0, 0.0, 0.0);

    // --- 1. AMBIENT (Światło otoczenia) ---
    Color ambient = mat.baseColor * mat.Ka;
    baseColor += ambient;

    // znormalizowane wektory: Normalna (N) i Wektor Kamery/Widza (V)
    Vector N = hit.intersectionLPOINTNormal.normalized();
    Vector V = (ray.origin() - hit.LPOINT).normalized();

    // MODEL OŚWIETLENIA (Iteracja po wszystkich źródłach w scenie)
    for (const auto& light : scene.getLights()) {

        Vector L = light->getDirectionFrom(hit.LPOINT).normalized();
        double distanceToLight = light->getDistanceFrom(hit.LPOINT);
        Color lightIntensity = light->getIntensityAt(hit.LPOINT);

        // CIENIE (Shadow Rays) 
        // Dodanie małego przesunięcia (N * 0.001) zapobiega zjawisku samocieniwoania,
        // wynikającemu z precyzji zmiennoprzecinkowej.
        Ray shadowRay(hit.LPOINT + (N * 0.001), L);
        IntersectionResult shadowHit = scene.intersect(shadowRay, 0.001, distanceToLight);

        // Jeśli coś stoi na drodze do światła (przed żarówką) - punkt leży w cieniu.
        if (shadowHit.type != MISS) {
            continue;
        }

        // --- 3. DIFFUSE (Prawo Lamberta) ---
        // Natężenie matowego światła jest proporcjonalne do cosinusa kąta padania (iloczyn skalarny N*L).
        double nDotL = std::max(0.0, N.dotProduct(L));
        Color diffuse = mat.baseColor * lightIntensity * (mat.Kd * nDotL);
        baseColor += diffuse;

        // SPECULAR (Rozbłysk Lustrzany
        // Oblicza wektor odbicia światła i porównuje go z wektorem kamery.
        if (nDotL > 0.0) {
            Vector R = (N * (2.0 * nDotL) - L).normalized();
            double rDotV = std::max(0.0, R.dotProduct(V));
            double specularFactor = std::pow(rDotV, mat.n); // 'n' to stopień zogniskowania rozbłysku

            specularColor += lightIntensity * (mat.Ks * specularFactor);
        }
    }

    //  REKURENCJA OPTYCZNA (Odbicia geometryczne i refrakcja świetlna) 
    Color reflectedColor(0.0, 0.0, 0.0);
    Color refractedColor(0.0, 0.0, 0.0);

    if (depth > 0) {
        Vector I = ray.direction().normalized(); // Incydent - wektor wpadający
        Vector N_norm = hit.intersectionLPOINTNormal.normalized();

        // IDEALNE LUSTRO (Reflection) 
        if (mat.reflectivity > 0.0) {
            Vector R = (I - N_norm * (2.0 * I.dotProduct(N_norm))).normalized();
            Ray reflectedRay(hit.LPOINT + (N_norm * 0.001), R); // Odsunięcie w stronę normalnej
            reflectedColor = trace(reflectedRay, scene, depth - 1);
        }

        // DIELEKTRYKI (Szkło 
        if (mat.transparency > 0.0) {
            auto n1 = 1.0;         // IOR powietrza
            auto n2 = mat.ior;     // IOR obiektu
            auto n_refract = N_norm;

            auto cosI = I.dotProduct(N_norm);

            // Analiza kierunku promienia (Wchodzi do szkła czy z niego wychodzi)
            if (cosI > 0.0) {
                std::swap(n1, n2);         // Zamiana współczynników ośrodków
                n_refract = N_norm * -1.0; // Odwrócenie normalnej do wnętrza bryły
            } else {
                cosI = -cosI;
            }

            auto eta = n1 / n2;

            // Wyróżnik pod pierwiastkiem. Jego wartość determinuje fizykę załamania.
            auto k = 1.0 - eta * eta * (1.0 - cosI * cosI);

            if (k >= 0.0) {
                // Promień załamuje się i przechodzi przez ośrodek.
                // (Jeśli k < 0, mamy do czynienia z Całkowitym Wewnętrznym Odbiciem)
                auto T = (I * eta) + (n_refract * (eta * cosI - std::sqrt(k)));
                T.normalize();

                // Ray wnika wgłąb obiektu, więc epsilon ma znak minus względem oryginalnej normalnej
                Ray refractedRay(hit.LPOINT - (n_refract * 0.001), T);
                refractedColor = trace(refractedRay, scene, depth - 1);
            }
        }
    }

  
    // Jeśli obiekt jest szkłem (transparency = 1.0), to localWeight spada do 0.0,
    // wygaszając całkowicie fizyczny, wbudowany kolor obiektu (Diffuse).
    double localWeight = 1.0 - mat.reflectivity - mat.transparency;
    if (localWeight < 0.0) localWeight = 0.0;

    finalColor = (baseColor * localWeight) +
                 specularColor + // Rozbłysk lustrzany dodajemy osobno 
                 (reflectedColor * mat.reflectivity) +
                 (refractedColor * mat.transparency);
    

    finalColor.setRed(std::min(1.0, finalColor.red()));
    finalColor.setGreen(std::min(1.0, finalColor.green()));
    finalColor.setBlue(std::min(1.0, finalColor.blue()));
    
    return finalColor;
}