#include "Renderer.h"
#include <execution> 
#include <numeric> 
#include <pplinterface.h>

void Renderer::render(const Scene& scene, const Camera& camera, Image& image,
                      const std::vector<Sample2D>& samples) const {
    
    auto width = image.getWidth();
    auto height = image.getHeight();
    
    // Inicjalizacja wektora przechowującego numery wierszy obrazu
    std::vector<int> rows(height);
    std::iota(rows.begin(), rows.end(), 0);
    
    // Zrównoleglenie obliczeń (wielowątkowość) - każdy wiersz (y) może być liczony na osobnym rdzeniu procesora
    std::for_each(std::execution::par, rows.begin(), rows.end(), [&](int y) {
        for (int x = 0; x < width; ++x) {
            // Dla każdego piksela wyliczamy jego kolor, a następnie zapisujemy go do obrazu
            Color pixelColor = renderPixel(x, y, scene, camera, width, height, samples);
            image.setPixel(x, y, pixelColor);
        }
    });
}

Color Renderer::renderPixel(int x, int y, const Scene& scene, const Camera& camera,
    int imageWidth,int imageHeight, const std::vector<Sample2D>& samples) const {
    
    Color sum(0.0, 0.0, 0.0);
    
    // Algorytm antyaliasingu: wypuszczamy wiele promieni (próbek) dla jednego piksela
    for (const Sample2D& sample : samples) {
        Ray ray = camera.generateRay(x, y, imageWidth, imageHeight, sample.x, sample.y);
        
        // Rozpoczynamy śledzenie promienia. depth = 3 to maksymalna ilość rekurencji (odbić/załamań)
        // Przy kolejnych wywołaniach energia spada, więc po 3 odbiciach różnica koloru jest już marginalna.
        sum += trace(ray, scene, 5); 
    }
    
    // Uśredniamy zebrane kolory ze wszystkich próbek tego piksela
    return sum / static_cast<double>(samples.size());
}

Color Renderer::trace(const Ray& ray, const Scene& scene, int depth) const {
    // Sprawdzamy przecięcie promienia ze wszystkimi obiektami na scenie. 
    // Minimalny dystans 0.001 zapobiega błędom precyzji, maksymalny 1e9 to "nieskończoność".
    IntersectionResult hit = scene.intersect(ray, 0.001, 1e9); 
    
    if (hit.type != MISS) {
        // Jeśli w coś trafiliśmy, obliczamy fizykę oświetlenia w tym punkcie
        return shade(hit, scene, ray, depth);
    }
    
    // Kolor tła, jeśli promień wyleciał w próżnię
    return Color(0.1, 0.1, 0.1);
}

Color Renderer::shade(const IntersectionResult& hit, const Scene& scene, const Ray& ray, int depth) const {
    Material mat = hit.material;
    Color finalColor(0.0, 0.0, 0.0);
    Color localColor(0.0, 0.0, 0.0); // Przechowuje kolor wyliczony z modelu Phonga (światło bezpośrednie)
    
    // --- 1. AMBIENT: Zawsze obecne światło rozproszone ---
    // Symuluje uproszczone oświetlenie globalne, aby obiekty w cieniu nie były smoliście czarne.
    Color ambient = mat.baseColor * mat.Ka;
    localColor += ambient;
    
    // Znormalizowany wektor normalny (N) w punkcie przecięcia
    Vector N = hit.intersectionLPOINTNormal.normalized();
    
    // Wektor kierunku do obserwatora / kamery (V) - odwrócony wektor promienia
    Vector V = (ray.origin() - hit.LPOINT).normalized(); 
    
    // --- MODEL PHONGA I CIENIE ---
    // Iterujemy przez wszystkie światła na scenie
    for (const auto& light : scene.getLights()) {
        
        // Wektor kierunkowy do światła (L) oraz odległość
        Vector L = light->getDirectionFrom(hit.LPOINT).normalized();
        double distanceToLight = light->getDistanceFrom(hit.LPOINT);
        Color lightIntensity = light->getIntensityAt(hit.LPOINT);
        
        // --- 2. CIENIE (Shadow Ray) ---
        // Generujemy promień z punktu przecięcia w stronę światła.
        // Używamy epsilona (hit.LPOINT + N * 0.001), aby przesunąć punkt minimalnie
        // nad powierzchnię figury i uniknąć "trądziku cieni" (samozacienienia geometrii).
        Ray shadowRay(hit.LPOINT + (N * 0.001), L);
        
        // Szukamy przeszkód tylko do odległości źródła światła
        IntersectionResult shadowHit = scene.intersect(shadowRay, 0.001, distanceToLight);
        
        if (shadowHit.type != MISS) {
            // Coś stoi na drodze do światła. Ten punkt jest w cieniu.
            // Ignorujemy oświetlenie bezpośrednie z tego źródła światła, zostaje sam ambient.
            continue; 
        }
        
        // --- 3. DIFFUSE (Model Lamberta) ---
        // Iloczyn skalarny (L * N) określa kąt padania światła.
        // std::max ucina wartości ujemne (gdy światło znajduje się z tyłu obiektu).
        double nDotL = std::max(0.0, N.dotProduct(L));
        Color diffuse = mat.baseColor * lightIntensity * (mat.Kd * nDotL);
        
        // --- 4. SPECULAR (Rozbłysk Phonga) ---
        Color specular(0.0, 0.0, 0.0);
        if (nDotL > 0.0) { // Liczymy odbicie lustrzane z żarówki tylko na oświetlonej połowie obiektu
            // R = 2 * (N * L) * N - L
            Vector R = (N * (2.0 * nDotL) - L).normalized();
            
            // Badamy, jak dobrze wektor odbicia trafia prosto w kamerę (V)
            double rDotV = std::max(0.0, R.dotProduct(V));
            
            // Podnosimy do potęgi 'n' (Shininess), aby zwęzić i wyostrzyć rozbłysk
            double specularFactor = std::pow(rDotV, mat.n);
            specular = lightIntensity * (mat.Ks * specularFactor);
        }
        
        // Dodajemy światło z tej żarówki do sumy oświetlenia lokalnego
        localColor += (diffuse + specular);
    }
    
    // --- REKURENCJA: ODBICIA I ZAŁAMANIA (ŚLEDZENIE WTÓRNE) ---
    Color reflectedColor(0.0, 0.0, 0.0);
    Color refractedColor(0.0, 0.0, 0.0);
    
    // Warunek ograniczający rekurencję zapobiega przepełnieniu stosu i nieskończonym obliczeniom
    if (depth > 0) {
        Vector I = ray.direction().normalized(); // Znormalizowany wektor kierunku promienia wpadającego
        Vector N = hit.intersectionLPOINTNormal.normalized();
       
        // --- LUSTRO (Odbicie idealne) ---
        if (mat.reflectivity > 0.0) {
            // Obliczamy idealny wektor odbicia względem normalnej: R = I - 2(I * N)N
            Vector R = (I - N * (2.0 * I.dotProduct(N))).normalized();
            
            // Wypuszczamy nowy promień odbity przesunięty o epsilon NAD powierzchnię (w stronę normalnej)
            Ray reflectedRay(hit.LPOINT + (N * 0.001), R);
            reflectedColor = trace(reflectedRay, scene, depth - 1);
        }
        
        // --- SZKŁO (Refrakcja - Prawo Snella) ---
        if (mat.transparency > 0.0) {
            auto n1 = 1.0;          // Współczynnik załamania ośrodka pierwotnego (zakładamy powietrze/próżnię)
            auto n2 = mat.ior;      // Współczynnik załamania docelowego materiału (np. 1.5 dla szkła)
            auto n_refract = N;     // Kopia wektora normalnego
           
            // cosI to rzut promienia na normalną, określa kąt wpadania
            auto cosI = I.dotProduct(N);
           
            // KRYTYCZNY TEST: Sprawdzamy, w którą stronę przechodzi światło
            if (cosI > 0.0) {
                // Skoro kosinus jest dodatni, wektor I oraz wektor normalny N celują w podobnym kierunku.
                // Oznacza to, że promień jest W ŚRODKU bryły i próbuje wyjść na zewnątrz.
                std::swap(n1, n2);    // Zamieniamy ośrodki: teraz przechodzimy ze szkła do powietrza
                n_refract = N * -1.0; // Odwracamy wektor normalny, aby celował do wnętrza
            } else {
                // Promień wchodzi z zewnątrz do środka bryły (standardowy przypadek)
                cosI = -cosI; 
            }
           
            // Eta to stosunek współczynników załamania
            auto eta = n1 / n2;
            
            // Wartość pod pierwiastkiem w równaniu na wektor załamany
            auto k = 1.0 - eta * eta * (1.0 - cosI * cosI);
           
            if (k < 0.0) {
                // k < 0.0 oznacza Całkowite Wewnętrzne Odbicie (TIR).
                // Kąt jest tak płaski, że światło nie jest w stanie opuścić gęstszego ośrodka 
                // i zachowuje się jak w światłowodzie. W tej uproszczonej wersji pomijamy ten promień.
            } else {
                // Wyznaczamy precyzyjny wektor załamanego promienia (T)
                auto T = (I * eta) + (n_refract * (eta * cosI - std::sqrt(k)));
                T.normalize();
                
                // Wypuszczamy nowy promień.
                // WAŻNE: W przeciwieństwie do cieni i luster, tutaj odejmujemy epsilon (-n_refract),
                // aby przesunąć punkt startowy W GŁĄB nowo wchodzonego ośrodka.
                Ray refractedRay(hit.LPOINT - (n_refract * 0.001), T);
                refractedColor = trace(refractedRay, scene, depth - 1);
            }
        }
    }
    
    // --- 5. MIKSOWANIE KOLORÓW I ZASADA ZACHOWANIA ENERGII ---
    // Obiekt łącznie nie może odbijać i przepuszczać więcej niż 100% fotonów (światła).
    double localWeight = 1.0 - mat.reflectivity - mat.transparency;
    if (localWeight < 0.0) localWeight = 0.0;
    
    // Wypadkowy kolor to połączenie światła lokalnego (Phong) i tego, co obiekt odbił z innych miejsc
    finalColor = (localColor * localWeight) + 
                 (reflectedColor * mat.reflectivity) + 
                 (refractedColor * mat.transparency);
                 
    // Zabezpieczenie przed "wypaleniem" kolorów - ograniczamy wartość kanałów RGB do 1.0
    finalColor.setRed(std::min(1.0, finalColor.red()));
    finalColor.setGreen(std::min(1.0, finalColor.green()));
    finalColor.setBlue(std::min(1.0, finalColor.blue()));
    
    return finalColor;
}