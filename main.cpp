#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Vector.h"
#include "Sphere.h"
#include "Plane.h"
#include "Color.h"
#include "Image.h"
#include "Camera.h"
#include "Scene.h"
#include "Sampler.h"
#include "Renderer.h"
#include "PointLight.h"

int main() {
    int width = 800;
    int height = 600;
    
    // Tło (czarne, jak próżnia za otwartym pokojem)
    Scene scene(Color(0.0, 0.0, 0.0)); 
    
    // ==========================================
    // 1. DEFINICJA MATERIAŁÓW DLA CORNELL BOX
    // ==========================================
    
    // Konstruktor: Material(Color, Ka, Kd, Ks, n, reflectivity, transparency, ior)
    
    // MATOWE ŚCIANY (Tylko rozpraszanie, zero odbić lustrzanych, zero refrakcji)
    Material matWhite(Color(0.8, 0.8, 0.8), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matRed  (Color(0.8, 0.1, 0.1), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matBlue (Color(0.1, 0.1, 0.8), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    
    // LUSTRO (reflectivity = 1.0, brak własnego koloru/diffuse)
    Material matMirror(Color(0.0, 0.0, 0.0), 0.0, 0.0, 1.0, 100.0, 1.0, 0.0, 1.0);
    
    // SZKŁO (transparency = 1.0, ior = 1.5, dodany silny rozbłysk Ks)
    Material matGlass (Color(0.0, 0.0, 0.0), 0.0, 0.0, 0.9, 100.0, 0.0, 1.0, 1.52);

    // ==========================================
    // 2. BUDOWA SCENY (GEOMETRIA)
    // ==========================================
    
    // PŁASZCZYZNY POKOJU:
    // Podłoga (y = -1.0, normalna w górę)
    scene.addPlane(Plane(Vector(0.0, 1.0, 0.0), 1.0, matWhite));
    // Sufit (y = 3.0, normalna w dół)
    scene.addPlane(Plane(Vector(0.0, -1.0, 0.0), 3.0, matWhite));
    // Lewa Ściana (x = -2.0, normalna w prawo)
    scene.addPlane(Plane(Vector(1.0, 0.0, 0.0), 2.0, matRed));
    // Prawa Ściana (x = 2.0, normalna w lewo)
    scene.addPlane(Plane(Vector(-1.0, 0.0, 0.0), 2.0, matBlue));
    // Tylna Ściana (z = 7.0, normalna w stronę kamery)
    scene.addPlane(Plane(Vector(0.0, 0.0, -1.0), 7.0, matWhite));

    // SFERY: (Zamieniona głębia na osi Z, by pasowała do referencyjnego obrazka)
    // Lewa Kula: Lustro (Cofnięta głębiej do tyłu pokoju, Z = 4.8)
    scene.addSphere(Sphere(Vector(-0.9, -0.1, 4.8), 0.9, matMirror));
    // Prawa Kula: Szkło (Wysunięta na pierwszy plan, bliżej kamery, Z = 3.0)
    scene.addSphere(Sphere(Vector(0.7, -0.1, 3.0), 0.9, matGlass));

    // ==========================================
    // 3. ŹRÓDŁO ŚWIATŁA
    // ==========================================
    // Światło powieszone pod samym sufitem pokoju (y = 2.9)
    std::shared_ptr<Light> mainLight = std::make_shared<PointLight>(
        Vector(0.0, 2.9, 4.0), 
        Color(1.0, 1.0, 1.0)
    );
    scene.addLight(mainLight);

    // ==========================================
    // 4. KONFIGURACJA KAMERY
    // ==========================================
    // Wchodzimy kamerą do środka pudełka (z = -1.0), trochę nad podłogą (y = 0.5)
    Vector eye(0.0, 0.5, -1.0); 
    Vector target(0.0, 0.5, 4.0);
    Vector up(0.0, 1.0, 0.0);
    
    // Kamera perspektywiczna (Finałowa jakość)
    Camera camPersp(CameraType::PERSPECTIVE, eye, target, up, 70.0, 5.0);
    
    // Kamera ortograficzna (Szybki test). Skala ustawiona na 6.0, by objąć całą scenę.
    Camera camOrtho(CameraType::ORTHOGRAPHIC, eye, target, up, 70.0, 6.0);
    
    Renderer renderer;
    std::cout << "Start" << std::endl;
    
    // --- Render 1: Szybki podgląd (Kamera Ortograficzna, 1 próbka/piksel) ---
    std::cout << "\n[1/2] cornell_box_ortho_fast.ppm" << std::endl;
    Image img1(width, height);
    std::vector<Sample2D> samples1 = Sampler::makeCenterSample();
    renderer.render(scene, camOrtho, img1, samples1);
    img1.savePPM("cornell_box_ortho_fast.ppm");
    std::cout << "Saved" << std::endl;
    
    // --- Render 2: Najwyższa jakość (Kamera Perspektywiczna, Jittered 4x4 = 16 próbek/piksel) ---
    std::cout << "\n[2/2] cornell_box_persp_hq.ppm" << std::endl;
    Image img2(width, height);
    std::vector<Sample2D> samples2 = Sampler::makeJitteredSamples(4); 
    renderer.render(scene, camPersp, img2, samples2);
    img2.savePPM("cornell_box_persp_hq.ppm"); 
    std::cout << "Saved" << std::endl;

    return 0;
}