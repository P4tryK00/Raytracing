#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Triangle.h"
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
    
    // Tło (czarne, wylatujące poza pokój promienie wyrenderują pustkę)
    Scene scene(Color(0.0, 0.0, 0.0)); 
    
    // ==========================================
    // 1. DEFINICJA MATERIAŁÓW DLA CORNELL BOX
    // ==========================================
    Material matWhite(Color(0.8, 0.8, 0.8), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matRed  (Color(0.8, 0.1, 0.1), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matBlue (Color(0.1, 0.1, 0.8), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matBlack(Color(0.1, 0.1, 0.1), 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matMirror(Color(0.0, 0.0, 0.0), 0.0, 0.0, 1.0, 100.0, 1.0, 0.0, 1.0);
    Material matGlass (Color(0.0, 0.0, 0.0), 0.0, 0.0, 0.9, 100.0, 0.0, 1.0, 1.5);

    // ==========================================
    // 2. BUDOWA SCENY (SIATKA TRÓJKĄTÓW)
    // ==========================================
    
    
    Vector FBL(-3.0, -1.0, -2.0); // Przód-Dół-Lewo
    Vector FBR( 3.0, -1.0, -2.0); // Przód-Dół-Prawo
    Vector FTL(-3.0,  3.0, -2.0); // Przód-Góra-Lewo
    Vector FTR( 3.0,  3.0, -2.0); // Przód-Góra-Prawo

    Vector BBL(-3.0, -1.0,  7.0); // Tył-Dół-Lewo
    Vector BBR( 3.0, -1.0,  7.0); // Tył-Dół-Prawo
    Vector BTL(-3.0,  3.0,  7.0); // Tył-Góra-Lewo
    Vector BTR( 3.0,  3.0,  7.0); // Tył-Góra-Prawo

    // PODŁOGA (Kolejność wierzchołków ustala wektor normalny skierowany do góry)
    scene.addTriangle(Triangle(FBL, BBL, BBR, matWhite));
    scene.addTriangle(Triangle(FBL, BBR, FBR, matWhite));

    // SUFIT (Normalna skierowana w dół)
    scene.addTriangle(Triangle(FTL, FTR, BTR, matBlack));
    scene.addTriangle(Triangle(FTL, BTR, BTL, matBlack));

    // LEWA ŚCIANA (Niebieska - normalna skierowana w prawo)
    scene.addTriangle(Triangle(FBL, FTL, BTL, matBlue));
    scene.addTriangle(Triangle(FBL, BTL, BBL, matBlue));

    // PRAWA ŚCIANA (Czerwona - normalna skierowana w lewo)
    scene.addTriangle(Triangle(FBR, BBR, BTR, matRed));
    scene.addTriangle(Triangle(FBR, BTR, FTR, matRed));

    // TYLNA ŚCIANA (Biała - normalna skierowana w stronę kamery)
    scene.addTriangle(Triangle(BBL, BTL, BTR, matWhite));
    scene.addTriangle(Triangle(BBL, BTR, BBR, matWhite));

    // SFERY: 
    scene.addSphere(Sphere(Vector(-1.4, -0.1, 4.0), 0.9, matGlass));    
    scene.addSphere(Sphere(Vector(1.2, -0.1, 5.0), 0.9, matMirror));

    // ==========================================
    // 3. ŹRÓDŁO ŚWIATŁA
    // ==========================================
    std::shared_ptr<Light> mainLight = std::make_shared<PointLight>(
        Vector(0.0, 2.5, 4.0), 
        Color(1.0, 1.0, 1.0)
    );
    scene.addLight(mainLight);

    // ==========================================
    // 4. KONFIGURACJA KAMERY
    // ==========================================
    Vector eye(0.0, 0.5, -1.0); 
    Vector target(0.0, 0.5, 4.0);
    Vector up(0.0, 1.0, 0.0);
    
    Camera camPersp(CameraType::PERSPECTIVE, eye, target, up, 70.0, 5.0);
    
    Renderer renderer;
    std::cout << "Start" << std::endl;
    std::cout << "\n[1/1] cornell_box_persp_hq.ppm" << std::endl;
    Image img2(width, height);
    std::vector<Sample2D> samples2 = Sampler::makeJitteredSamples(4); 
    renderer.render(scene, camPersp, img2, samples2);
    img2.savePPM("cornell_box_persp_hq.ppm"); 
    std::cout << "Saved" << std::endl;

    return 0;
}