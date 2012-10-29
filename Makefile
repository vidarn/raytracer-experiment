OUT = raytracer
OBJ = app.o
OBJ += utils/vec3.o utils/point.o utils/ray.o utils/rgba.o utils/normal.o
OBJ += geom/geometricObject.o geom/plane.o geom/sphere.o
OBJ += scene/scene.o
OBJ += viewPlane/viewPlane.o
OBJ += tracer/tracer.o
OBJ += sampler/sampler.o
OBJ += shadeRec/shadeRec.o
OBJ += bucket/bucket.o
FLG = -g -pthread
LIB = -ltiff
$(OUT):$(OBJ)
	g++ -o $(OUT) $(OBJ) $(FLG) $(LIB)

%.o : %.cpp %.h
	g++ $(FLG) $(INC) -c $< -o $@

clean:
	rm *.o
	rm */*.o
