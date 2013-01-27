#include <iostream>
#include "app.h"
#include "scene/scene.h"
#include "viewPlane/viewPlane.h"
#include "tracer/tracer.h"
#include "utils/matrix4x4.h"
#include "file/file.h"
#include "file/image.h"
#include "brdf/lambert.h"
#include <Magick++.h>

int main(int argc, char** argv)
{
    Magick::InitializeMagick(*argv);
	const char * filename = "/tmp/out.png";
	if(argc == 2){
		filename = argv[1];
	}
    ViewPlane viewPlane(filename);
    Scene scene;
    scene.build(viewPlane);
	viewPlane.saveToImage();
    Tracer tracer(&scene, &viewPlane);
    tracer.render();
}
