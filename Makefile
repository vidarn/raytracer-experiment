OUT = raytracer
OBJ = app.o
OBJ += utils/vec3.o utils/ray.o utils/rgba.o utils/matrix4x4.o
OBJ += geom/geometricObject.o geom/mesh.o geom/triangle.o
OBJ += aggregates/aaBoundingBox.o aggregates/kdTree.o aggregates/bvh.o
OBJ += scene/scene.o
OBJ += viewPlane/viewPlane.o
OBJ += tracer/tracer.o
OBJ += sampler/sampler.o sampler/randomSampler.o sampler/stratifiedSampler.o sampler/sampling.o
OBJ += shadeRec/shadeRec.o
OBJ += bucket/bucket.o
OBJ += material/material.o
OBJ += brdf/brdf.o brdf/lambert.o brdf/phong.o brdf/specularReflection.o
OBJ += lights/light.o lights/directional.o lights/point.o lights/area.o
OBJ += settings/settings.o
OBJ += file/file.o
FLG = -pthread -O3
LIB = -ltiff

$(OUT):$(OBJ)
	g++ -o $(OUT) $(OBJ) $(FLG) $(LIB)

%.o : %.cpp %.h Makefile
	g++ $(FLG) $(INC) -c $< -o $@

clean:
	rm `find . -name '*.o'`
	rm $(OUT)
