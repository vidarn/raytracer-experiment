#include <iostream>
#include "app.h"
#include "scene/scene.h"
#include "viewPlane/viewPlane.h"
#include "tracer/tracer.h"

int main(int argc, char** argv)
{
    Scene scene;
    scene.build();
    ViewPlane viewPlane(20,20,1.0,1.0);
    Tracer tracer(&scene, &viewPlane);
    tracer.render();
    std::cout << viewPlane;
}
