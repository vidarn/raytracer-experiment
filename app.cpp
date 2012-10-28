#include <iostream>
#include "app.h"
#include "scene/scene.h"
#include "viewPlane/viewPlane.h"
#include "tracer/tracer.h"

int main(int argc, char** argv)
{
    Scene scene;
    scene.build();
    ViewPlane viewPlane(512,512,1.0,1.0);
	Sampler sampler(3);
    Tracer tracer(&scene, &viewPlane, &sampler);
    tracer.render();
    //std::cout << viewPlane;
	viewPlane.saveToTiff("out.tif");
}
