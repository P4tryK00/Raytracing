#include <iostream>
#include <numbers>
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"

int main() {
    Vector vec1(0,3,0);
    Vector vec2(5,5,0);
    Vector vec3;
    Vector vec4;

    vec3+=vec1;
    vec4+=vec2;

    std::cout<<vec3.x;
    std::cout<<" "<<vec3.y;
    std::cout<<" "<<vec3.z<<std::endl;
    std::cout<<vec4.x;
    std::cout<<" "<<vec4.y;
    std::cout<<" "<<vec4.z<<std::endl;

    // Task 3
    auto alpha = acos(  vec1.dotProduct( vec2 ) / ( vec1.length() * vec2.length() ) );
    std::cout<<"3: "<<alpha * (180/std::numbers::pi)<<" deegres"<<std::endl;


    // Task 4
    Vector vec5(4,5,1);
    Vector vec6(4,1,3);

    auto crossProd56 = vec5.crossProduct(vec6);
    std::cout<<"4: "<<crossProd56.x<<" "<<crossProd56.y<<" "<<crossProd56.z<<std::endl;

    //Task 5
    auto normalizedCrossProd56 = crossProd56.normalized();
    std::cout<<"5: "<<normalizedCrossProd56.x<<normalizedCrossProd56.y<<normalizedCrossProd56.z<<std::endl;

    //Task 7
    Vector center(0,0,0);
    Sphere s1(center,10);
    Vector origin(0,0,-20);










    return 0;
}