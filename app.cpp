#include <iostream>
#include "app.h"
#include "scene/scene.h"
#include "viewPlane/viewPlane.h"
#include "tracer/tracer.h"
#include "utils/matrix4x4.h"
#include "file/file.h"

int main(int argc, char** argv)
{
    Scene scene;
    scene.build();
    ViewPlane viewPlane(512,512,1.0,1.0);
    Tracer tracer(&scene, &viewPlane);
    tracer.render();
}
