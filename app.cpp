#include <iostream>
#include "app.h"
#include "utils/vec3.h"
#include "utils/point.h"
#include "utils/ray.h"

int main(int argc, char** argv)
{
    Vec3 a;
    Point b;
    a[0] = 2.4;
    a[1] = 1.4;
    a[2] = 1.1;
    b[0] = 2.4;
    b[1] = 1.4;
    b[2] = 1.1;
    a.normalize();
    Ray r(b,a);
    std::cout << "hej hej!" << std::endl;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << r << std::endl;
}
