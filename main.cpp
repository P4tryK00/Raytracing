#include <iostream>
#include <numbers>

#include "Plane.h"
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include  "Plane.h"

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
    std::cout<<"5: "<<normalizedCrossProd56.x <<" "
    <<normalizedCrossProd56.y << " "
    <<normalizedCrossProd56.z<<std::endl;

    //Task 7
    Vector center(0,0,0);
    Sphere s1(center,10);
    Vector origin(0,0,-20);
    Vector dir(0,0,1);
    Ray r1(origin,dir);
    Vector dir2(0,20,0);
    Ray r2(origin, dir2);

    IntersectionResult res1 = s1.Hit(r1, 0.001, INFINITY);
    IntersectionResult res2 = s1.Hit(r2, 0.001, INFINITY);
    if (res1.type == HIT  ) {
        std::cout<<"res1 LPOINT: "<< res1.LPOINT<<std::endl;
    }else {
        std::cout<<"MISS"<<std::endl;
    }

    if ( res2.type == HIT  ) {
        std::cout<<"res 2LPOINT: "<< res2.LPOINT<<std::endl;
    }else {
        std::cout<<"MISS"<<std::endl;
    }

    //task 12
    Vector originR3(10, -20, 0);
    Vector dirR3(0,1,0);
    Ray r3(originR3,dirR3);

    IntersectionResult res3 = s1.Hit(r3, 0.001, INFINITY);

    if (res3.type == HIT  ) {
        std::cout<<"res 3LPOINT: "<< res3.LPOINT<<std::endl;
    }else {
        std::cout<<"MISS"<<std::endl;
    }

    Vector planeNormal(0, 1, 1);
    Plane p(planeNormal, 0.0);

    std::cout << "13 : " << p << std::endl;

    //task 14
    IntersectionResult resPlane = p.Intersects(r2, 0.0);
    if (resPlane.type == HIT || resPlane.type == INSIDE_PRIMITIVE ) {
        std::cout<<"r2 plane LPOINT:"<<resPlane.LPOINT<<std::endl;
    }else {
        std::cout<<"MISS"<<std::endl;
    }










    return 0;
}