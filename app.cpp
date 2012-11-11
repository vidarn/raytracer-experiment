#include <iostream>
#include "app.h"
#include "scene/scene.h"
#include "viewPlane/viewPlane.h"
#include "tracer/tracer.h"
#include "utils/matrix4x4.h"

int main(int argc, char** argv)
{
    Scene scene;
    scene.build();
    ViewPlane viewPlane(256,256,1.0,1.0);
	Sampler sampler(2);
    Tracer tracer(&scene, &viewPlane, &sampler);
    tracer.render();
	viewPlane.saveToTiff("out.tif");
}
