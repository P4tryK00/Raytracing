#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <vector>
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Color.h"
#include "Ray.h"
#include "IntersectionResult.h"
#include <memory>
#include "Light.h"

// Główny kontener wirtualnego środowiska trójwymiarowego (Scene Graph / World).
// Pełni rolę bazy danych przechowującej wszystkie obiekty fizyczne oraz źródła światła.
// Odpowiada za zarządzanie pamięcią sceny oraz agregację testów kolizji dla promieni.
class Scene {
public:
    // Inicjalizuje pustą scenę z określonym kolorem tła (np. nieboskłonu).
    Scene(const Color& backgroundColor);

    // --- METODY POPULACJI SCENY ---
    // Funkcje wczytujące geometrię do pamięci silnika.
    void addSphere(const Sphere& sphere);
    void addPlane(const Plane& plane);
    void addTriangle(const Triangle& triangle);

    // Rejestracja polimorficznego źródła światła (PointLight, SpotLight itp.).
    // Użycie std::shared_ptr jest konieczne, aby silnik nie uciął (Object Slicing) specyficznych
    // dla danego typu światła danych (np. kątów stożka w reflektorze) podczas rzutowania w górę do klasy Light.
    void addLight(std::shared_ptr<Light> light);

    // Udostępnia listę wszystkich aktywnych świateł silnikowi oświetlenia (Renderer::shade).
    const std::vector<std::shared_ptr<Light>>& getLights() const;

    // Najważniejsza metoda optyczna sceny (Scene Traversal).
    // Testuje kolizję przekazanego promienia ze wszystkimi obiektami w scenie i zwraca
    // dane o absolutnie najbliższym punkcie uderzenia w przedziale <t_min, t_max>.
    IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;

    // Kolor tła. Zwracany przez silnik, gdy promień ucieknie w nieskończoność (brak kolizji z geometrią).
    Color backgroundColor;

private:
    // Płaskie kontenery przechowujące geometrię w pamięci operacyjnej (podejście O(N)).
    // W profesjonalnych, dużych silnikach zastępowane są one zazwyczaj jedną wielką
    // strukturą przestrzenną Bounding Volume Hierarchy (BVH) lub siatką (Grid),
    // aby drastycznie skrócić czas szukania kolizji (do O(log N)).
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Triangle> triangles;

    // Wektor trzymający inteligentne wskaźniki do świateł, gwarantujący automatyczne
    // zwolnienie pamięci po destrukcji obiektu Scene.
    std::vector<std::shared_ptr<Light>> lights;
};

#endif //RAYTRACING_SCENE_H