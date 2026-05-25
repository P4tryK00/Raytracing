#include <iostream>
#include <string>
#include <vector>
#include "Vector.h"
#include "Sphere.h"
#include "Plane.h"
#include "Color.h"
#include "Image.h"
#include "Camera.h"
#include "Scene.h"
#include "Sampler.h"
#include "Renderer.h"

int main() {
    int width = 800;
    int height = 600;
    
    
    Scene scene(Color(0.2, 0.2, 0.2)); 
    
    scene.addSphere(Sphere(Vector(-0.6, 0.0, -4.0), 1.2, Color(0.0, 0.0, 1.0)));
    scene.addSphere(Sphere(Vector(1.2, 0.0, -6.0), 1.0, Color(1.0, 0.0, 0.0)));

    
    Vector eye(0.0, 0.0, 0.0); 
    Vector target(0.0, 0.0, -5.0);
    Vector up(0.0, 1.0, 0.0);
    
    Renderer renderer;
    std::cout << "Start" << std::endl;
    
    std::cout << "\n[1/3] perspective_1spp.ppm" << std::endl;
    Image img1(width, height);
    Camera camPersp(CameraType::PERSPECTIVE, eye, target, up, 60.0, 5.0);
    std::vector<Sample2D> samples1 = Sampler::makeCenterSample();
    
    renderer.render(scene, camPersp, img1, samples1);
    img1.savePPM("perspective_1spp.ppm");
    std::cout << "Saved" << std::endl;
    
    // render 2
  
    std::cout << "\n[2/3] orthographic_1spp.ppm" << std::endl;
    Image img2(width, height);
    Camera camOrtho(CameraType::ORTHOGRAPHIC, eye, target, up, 60.0, 5.0);
    std::vector<Sample2D> samples2 = Sampler::makeCenterSample(); // Też 1 próbka
    
    renderer.render(scene, camOrtho, img2, samples2);
    img2.savePPM("orthographic_1spp.ppm");
    std::cout << "Saved" << std::endl;

    //render 3
    std::cout << "\n[3/3] perspective_jittered_4x4_aa.ppm" << std::endl;
    Image img3(width, height);
    std::vector<Sample2D> samples3 = Sampler::makeJitteredSamples(4); // 16 próbek
    
    renderer.render(scene, camPersp, img3, samples3);
    img3.savePPM("perspective_4x4_aa.ppm");
    std::cout << "Saved"<<std::endl;
    
    //render 4
    std::cout << "\n[4/3] perspective_jittered_aa.ppm" << std::endl;
    Image img4(width, height);
    std::vector<Sample2D> samples4 = Sampler::makeJitteredSamples(1);
    renderer.render(scene, camPersp, img4, samples4);
    img4.savePPM("perspective_aa.ppm");
    std::cout << "Saved" << std::endl;
    // render 5
    std::cout << "\n[5/3] perspective_jittered_2x2_aa.ppm" << std::endl;
    Image img5(width, height);
    std::vector<Sample2D> samples5 = Sampler::makeJitteredSamples(2);
    renderer.render(scene, camPersp, img5, samples5);
    img4.savePPM("perspective_jittered_2x2_aa.ppm");
    std::cout << "Saved" << std::endl;
    
    //render 3
    std::cout << "\n[6/3] perspective_regular_4x4_aa.ppm" << std::endl;
    Image img6(width, height);
    std::vector<Sample2D> samples6 = Sampler::makeRegularSample(4); // 16 próbek
    
    renderer.render(scene, camPersp, img6, samples6);
    img3.savePPM("perspective_regular_4x4_aa.ppm");
    std::cout << "Saved"<<std::endl;
    
    //render 4
    std::cout << "\n[7/3] perspective_regular_aa.ppm" << std::endl;
    Image img7(width, height);
    std::vector<Sample2D> samples7 = Sampler::makeRegularSample(1);
    renderer.render(scene, camPersp, img7, samples7);
    img4.savePPM("perspective_regular_aa.ppm");
    std::cout << "Saved" << std::endl;
    // render 5
    std::cout << "\n[8/3] perspective_regular_2x2_aa.ppm" << std::endl;
    Image img8(width, height);
    std::vector<Sample2D> samples8 = Sampler::makeRegularSample(2);
    renderer.render(scene, camPersp, img8, samples8);
    img4.savePPM("perspective_regular_2x2_aa.ppm");
    std::cout << "Saved" << std::endl;
    

    return 0;
}