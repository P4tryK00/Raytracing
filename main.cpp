#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Vector.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Color.h"
#include "Image.h"
#include "Camera.h"
#include "Scene.h"
#include "Sampler.h"
#include "Renderer.h"
#include "PointLight.h"
#include "SpotLight.h"

// =========================================================
// FUNKCJA POMOCNICZA: Buduje pokój i renderuje podane światło
// =========================================================
void renderSetup(const std::string& filename, std::shared_ptr<Light> light, int width, int height) {
    Scene scene(Color(0.0, 0.0, 0.0));

    // --- 1. MATERIAŁY ---
    Material matWhite(Color(0.8, 0.8, 0.8), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matRed  (Color(0.8, 0.1, 0.1), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matBlue (Color(0.1, 0.1, 0.8), 0.2, 0.8, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matBlack(Color(0.1, 0.1, 0.1), 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
    Material matMirror(Color(0.0, 0.0, 0.0), 0.0, 0.0, 1.0, 100.0, 1.0, 0.0, 1.0);
    Material matGlass (Color(0.0, 0.0, 0.0), 0.0, 0.0, 0.9, 100.0, 0.0, 1.0, 1.5);

    // --- 2. GEOMETRIA POKOJU ---
    Vector FBL(-3.0, -1.0, -2.0); Vector FBR( 3.0, -1.0, -2.0);
    Vector FTL(-3.0,  3.0, -2.0); Vector FTR( 3.0,  3.0, -2.0);
    Vector BBL(-3.0, -1.0,  7.0); Vector BBR( 3.0, -1.0,  7.0);
    Vector BTL(-3.0,  3.0,  7.0); Vector BTR( 3.0,  3.0,  7.0);

    scene.addTriangle(Triangle(FBL, BBL, BBR, matWhite));
    scene.addTriangle(Triangle(FBL, BBR, FBR, matWhite));
    scene.addTriangle(Triangle(FTL, FTR, BTR, matBlack));
    scene.addTriangle(Triangle(FTL, BTR, BTL, matBlack));
    scene.addTriangle(Triangle(FBL, FTL, BTL, matBlue));
    scene.addTriangle(Triangle(FBL, BTL, BBL, matBlue));
    scene.addTriangle(Triangle(FBR, BBR, BTR, matRed));
    scene.addTriangle(Triangle(FBR, BTR, FTR, matRed));
    scene.addTriangle(Triangle(BBL, BTL, BTR, matWhite));
    scene.addTriangle(Triangle(BBL, BTR, BBR, matWhite));

    scene.addSphere(Sphere(Vector(-1.4, -0.1, 4.0), 0.9, matGlass));
    scene.addSphere(Sphere(Vector(1.2, -0.1, 5.0), 0.9, matMirror));

    // --- 3. ŚWIATŁO---
    scene.addLight(light);

    // --- 4. KAMERA I RENDERER ---
    Vector eye(0.0, 0.5, -1.0);
    Vector target(0.0, 0.5, 4.0);
    Vector up(0.0, 1.0, 0.0);
    Camera camPersp(CameraType::PERSPECTIVE, eye, target, up, 70.0, 5.0);

    Renderer renderer;
    Image img(width, height);
    std::vector<Sample2D> samples = Sampler::makeJitteredSamples(4);

    std::cout <<  filename << std::endl;
    renderer.render(scene, camPersp, img, samples);
    img.savePPM(filename);
    std::cout << "Saved file: " << filename << "\n" << std::endl;
}

// =========================================================
// GŁÓWNA PĘTLA PROGRAMU (BATCH RENDER)
// =========================================================
int main() {
    int width = 800;
    int height = 600;

    std::cout << "Start\n" << std::endl;

    // --- SCENA 1: Miękkie przejście ---
    auto lightTeatr = std::make_shared<SpotLight>(
        Vector(2.0, 2.9, 3.5), Vector(0.0, -1.0, 0.5), Color(1.0, 1.0, 1.0),
        10.0, 35.0, 1.0, 0.04, 0.01
    );
    renderSetup("cornell_teatr.ppm", lightTeatr, width, height);

    // --- SCENA 2: Twarde odcięcie ---
    auto lightLatarka = std::make_shared<SpotLight>(
        Vector(2.0, 2.9, 3.5), Vector(0.0, -1.0, 0.5), Color(1.0, 1.0, 1.0),
        15.0, 16.0, 1.0, 0.04, 0.01
    );
    renderSetup("cornell_latarka.ppm", lightLatarka, width, height);

    // --- SCENA 3: BAZA  ---
    auto lightPunktowe = std::make_shared<PointLight>(
        Vector(2.0, 2.9, 3.5), Color(1.0, 1.0, 1.0),
        1.0, 0.04, 0.01
    );
    renderSetup("cornell_punktowe.ppm", lightPunktowe, width, height);

    std::cout << "Stop" << std::endl;
    return 0;
}