OUT = raytracer
OBJ = app.o
OBJ += utils/vec3.o utils/point.o utils/ray.o utils/rgba.o utils/normal.o
OBJ += geom/geometricObject.o geom/plane.o
OBJ += scene/scene.o
OBJ += viewPlane/viewPlane.o
OBJ += tracer/tracer.o
FLG =
INC =
$(OUT):$(OBJ)
	g++ -o $(OUT) $(OBJ) $(FLG) $(INC)

%.o : %.cpp %.h
	g++ $(FLG) $(INC) -c $< -o $@

clean:
	rm *.o
	rm */*.o
