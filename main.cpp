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
    
    // Ciemne tło
    Scene scene(Color(0.1, 0.1, 0.1)); 
    
    // --- 1. DEFINICJA MATERIAŁÓW (Współczynniki Phonga) ---
    // Błyszcząca niebieska kula (wysokie odbicie lustrzane Ks i wysoka gładkość n)
    Material bluePlastic(Color(0.0, 0.0, 1.0), 0.2, 0.7, 0.8, 50.0);
    
    // Matowa czerwona kula (bardzo niskie odbicie lustrzane Ks, niska gładkość n)
    Material redMatte(Color(1.0, 0.2, 0.2), 0.2, 0.9, 0.1, 5.0);
    
    // Zielona podłoga, na której wyrenderują się cienie (Shadow Rays)
    Material greenFloor(Color(0.1, 0.8, 0.1), 0.2, 0.8, 0.2, 10.0);

    // --- 2. BUDOWA SCENY ---
    // Kule umieszczone są teraz w głębi sceny, na dodatniej osi Z (Z = 4.0 i Z = 6.0).
    scene.addSphere(Sphere(Vector(-0.6, 0.0, 4.0), 1.2, bluePlastic));
    scene.addSphere(Sphere(Vector(1.2, 0.0, 6.0), 1.0, redMatte));
    
    // Płaszczyzna z normalną skierowaną w górę (0.0, 0.9, 0.0 znormalizuje się do 0,1,0).
    // Parametr dystansu 1.2 matematycznie umieszcza ją pod kulami.
    scene.addPlane(Plane(Vector(0.0, 0.9, 0.0), 1.2, greenFloor));

    // --- 3. DODANIE ŚWIATŁA ---
    // Białe światło punktowe umieszczone w przestrzeni pomiędzy kamerą a kulami (Z = 1.0),
    // podniesione do góry (Y = 5.0) i przesunięte w prawo (X = 3.0).
    std::shared_ptr<Light> mainLight = std::make_shared<PointLight>(
        Vector(3.0, 5.0, 1.0), 
        Color(1.0, 1.0, 1.0)
    );
    scene.addLight(mainLight);

    // --- 4. KONFIGURACJA KAMER  ---
    // Kamera została podniesiona do góry (Y = 5.0) i cofnięta (Z = -5.0).
    Vector eye(0.0, 5.0, -5.0); 
    
    // Kamera patrzy w dół pod kątem, celując w głąb sceny (Z = 5.0).
    Vector target(0.0, 0.0, 5.0);
    
    // Globalny wektor wskazujący "górę" świata (Oś Y).
    Vector up(0.0, 1.0, 0.0);
    
    // Konfiguracja kamer
    Camera camPersp(CameraType::PERSPECTIVE, eye, target, up, 60.0, 5.0);
    Camera camOrtho(CameraType::ORTHOGRAPHIC, eye, target, up, 60.0, 5.0);
    
    Renderer renderer;
    std::cout << "Start" << std::endl;
    
    // --- Render 1 (Perspektywa, brak AA) ---
    std::cout << "\n[1/8] perspective_1spp.ppm" << std::endl;
    Image img1(width, height);
    std::vector<Sample2D> samples1 = Sampler::makeCenterSample();
    renderer.render(scene, camPersp, img1, samples1);
    img1.savePPM("perspective_1spp.ppm");
    std::cout << "Saved!" << std::endl;
    
    // --- Render 2 (Ortografia, brak AA) ---
    std::cout << "\n[2/8] orthographic_1spp.ppm" << std::endl;
    Image img2(width, height);
    std::vector<Sample2D> samples2 = Sampler::makeCenterSample();
    renderer.render(scene, camOrtho, img2, samples2);
    img2.savePPM("orthographic_1spp.ppm");
    std::cout << "Saved!" << std::endl;

    // --- Render 3 (Perspektywa, AA Jittered 4x4 - 16 próbek na piksel) ---
    std::cout << "\n[3/8] perspective_jittered_4x4_aa.ppm" << std::endl;
    Image img3(width, height);
    std::vector<Sample2D> samples3 = Sampler::makeJitteredSamples(4); 
    renderer.render(scene, camPersp, img3, samples3);
    img3.savePPM("perspective_jittered_4x4_aa.ppm"); 
    std::cout << "Saved" << std::endl;
    
    // --- Render 4 (Perspektywa, AA Jittered 1x1) ---
    std::cout << "\n[4/8] perspective_jittered_1x1_aa.ppm" << std::endl;
    Image img4(width, height);
    std::vector<Sample2D> samples4 = Sampler::makeJitteredSamples(1);
    renderer.render(scene, camPersp, img4, samples4);
    img4.savePPM("perspective_jittered_1x1_aa.ppm"); 
    std::cout << "Saved" << std::endl;

    // --- Render 5 (Perspektywa, AA Jittered 2x2 - 4 próbki na piksel) ---
    std::cout << "\n[5/8] perspective_jittered_2x2_aa.ppm" << std::endl;
    Image img5(width, height);
    std::vector<Sample2D> samples5 = Sampler::makeJitteredSamples(2);
    renderer.render(scene, camPersp, img5, samples5);
    img5.savePPM("perspective_jittered_2x2_aa.ppm"); 
    std::cout << "Saved" << std::endl;
    
    // --- Render 6 (Perspektywa, AA Regular 4x4) ---
    std::cout << "\n[6/8] perspective_regular_4x4_aa.ppm" << std::endl;
    Image img6(width, height);
    std::vector<Sample2D> samples6 = Sampler::makeRegularSample(4); 
    renderer.render(scene, camPersp, img6, samples6);
    img6.savePPM("perspective_regular_4x4_aa.ppm"); 
    std::cout << "Saved" << std::endl;
    
    // --- Render 7 (Perspektywa, AA Regular 1x1) ---
    std::cout << "\n[7/8] perspective_regular_1x1_aa.ppm" << std::endl;
    Image img7(width, height);
    std::vector<Sample2D> samples7 = Sampler::makeRegularSample(1);
    renderer.render(scene, camPersp, img7, samples7);
    img7.savePPM("perspective_regular_1x1_aa.ppm"); 
    std::cout << "Saved" << std::endl;

    // --- Render 8 (Perspektywa, AA Regular 2x2) ---
    std::cout << "\n[8/8] perspective_regular_2x2_aa.ppm" << std::endl;
    Image img8(width, height);
    std::vector<Sample2D> samples8 = Sampler::makeRegularSample(2);
    renderer.render(scene, camPersp, img8, samples8);
    img8.savePPM("perspective_regular_2x2_aa.ppm"); 
    std::cout << "Saved!" << std::endl;
    
    return 0;
}