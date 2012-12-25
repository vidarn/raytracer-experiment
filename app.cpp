#include <iostream>
#include "app.h"
#include "scene/scene.h"
#include "viewPlane/viewPlane.h"
#include "tracer/tracer.h"
#include "utils/matrix4x4.h"
#include "file/file.h"

int main(int argc, char** argv)
{
	const char * filename = "/tmp/out.tif";
	if(argc == 2){
		filename = argv[1];
	}
    ViewPlane viewPlane(filename);
    Scene scene;
    scene.build(viewPlane);
	viewPlane.saveToTiff();
    Tracer tracer(&scene, &viewPlane);
    tracer.render();
}
