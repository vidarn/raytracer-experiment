OUT = raytracer
SRC = app.cpp
SRC += utils/vec3.cpp utils/point.cpp utils/ray.cpp utils/rgba.cpp utils/normal.cpp
SRC += geom/geometricObject.cpp geom/plane.cpp
FLG =
INC =
all:
	g++ -o $(OUT) $(SRC) $(FLG) $(INC)
