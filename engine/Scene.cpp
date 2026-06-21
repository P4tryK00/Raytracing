#include "Scene.h"

// Inicjalizacja środowiska 3D. Kolor tła (backgroundColor) jest zwracany przez silnik,
// gdy promień nie uderzy w żadną geometrię (tzw. Void / Skybox).
Scene::Scene(const Color &backgroundColor) : backgroundColor(backgroundColor) {}

// --- METODY ZARZĄDZANIA PAMIĘCIĄ GEOMETRII ---
// Dodają prymitywy do wektorów. W profesjonalnych silnikach zamiast płaskich list
// stosuje się tu struktury przestrzenne (np. BVH lub KD-Tree) w celu redukcji
// złożoności obliczeniowej z O(N) do O(log N).

void Scene::addPlane(const Plane &plane) {
    planes.push_back(plane);
}

void Scene::addSphere(const Sphere &sphere) {
    spheres.push_back(sphere);
}

void Scene::addTriangle(const Triangle &triangle) {
    triangles.push_back(triangle);
}

// Rejestruje polimorficzne źródło światła (PointLight, SpotLight, itp.).
// Użycie std::shared_ptr zapobiega wyciekom pamięci (Memory Leaks) przy usuwaniu sceny.
void Scene::addLight(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

// Udostępnia listę świateł silnikowi cieniującemu (Renderer::shade).
const std::vector<std::shared_ptr<Light>>& Scene::getLights() const {
    return lights;
}

// Główny agregator fizyki kolizji w silniku (Scene Traversal).
// Przeprowadza test intersekcji danego promienia z absolutnie każdym obiektem na scenie.
IntersectionResult Scene::intersect(const Ray &ray, double t_min, double t_max) const {
    IntersectionResult closestHit;
    closestHit.type = MISS;

    // Inicjalizujemy najbliższy znaleziony dystans wartością maksymalną (np. nieskończonością 1e9).
    auto closestT = t_max;

    // --- TESTY KOLIZJI (Naiwne szukanie liniowe O(N)) ---
    // Optymalizacja: po znalezieniu jakiegokolwiek trafienia, przekazujemy do kolejnych testów
    // zaktualizowane 'closestT' zamiast 't_max'. Dzięki temu funkcje przecięcia wewnątrz figur
    // mogą natychmiast odrzucić uderzenia leżące dalej (matematyczny odpowiednik Z-Buffera).

    // 1. Test sfer
    for (const auto& sphere : spheres) {
        auto hit = sphere.intersect(ray, t_min, closestT);
        if (hit.type != MISS && hit.distance < closestT) {
            closestT = hit.distance; // Zawężenie przedziału poszukiwań
            closestHit = hit;        // Nadpisanie wyniku bliższym obiektem
        }
    }

    // 2. Test trójkątów (Mesh)
    for (const auto& triangle : triangles) {
        auto hit = triangle.intersect(ray, t_min, closestT);
        if (hit.type != MISS && hit.distance < closestT) {
            closestT = hit.distance;
            closestHit = hit;
        }
    }

    // 3. Test nieskończonych płaszczyzn
    for (const auto& plane : planes ) {
        auto hit = plane.intersect(ray, t_min, closestT);
        if (hit.type != MISS && hit.distance < closestT) {
            closestT = hit.distance;
            closestHit = hit;
        }
    }

    // Zwraca pełen pakiet danych (normalna, materiał, pozycja) dla absolutnie
    // najbliższego obiektu trafionego przez ten konkretny promień.
    return closestHit;
}