OUT = raytracer
OBJ = app.o
OBJ += utils/vec3.o utils/point.o utils/ray.o utils/rgba.o utils/normal.o utils/matrix4x4.o
OBJ += geom/geometricObject.o geom/plane.o geom/sphere.o geom/mesh.o
OBJ += geom/meshReaders/objReader.o
OBJ += aggregates/aaBoundingBox.o aggregates/collection.o aggregates/kdTree.o
OBJ += scene/scene.o
OBJ += viewPlane/viewPlane.o
OBJ += tracer/tracer.o
OBJ += sampler/sampler.o
OBJ += shadeRec/shadeRec.o
OBJ += bucket/bucket.o
OBJ += material/material.o
OBJ += brdf/brdf.o brdf/lambert.o brdf/phong.o
OBJ += lights/light.o lights/directional.o lights/point.o
OBJ += file/file.o
FLG = -g -pthread
LIB = -ltiff
$(OUT):$(OBJ)
	g++ -o $(OUT) $(OBJ) $(FLG) $(LIB)

%.o : %.cpp %.h
	g++ $(FLG) $(INC) -c $< -o $@

clean:
	rm `find . -name '*.o'`
	rm $(OUT)
