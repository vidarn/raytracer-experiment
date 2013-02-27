#include "file.h"
#include "../geom/mesh.h"
#include <cstring>
#include <vector>

File::File(const char *filename)
{
    m_filename = new char[strlen(filename)+1];
    strcpy(m_filename,filename);
    m_filename[strlen(filename)] = '\0';
}

void File::read(std::vector<GeometricObject *> &objects, std::vector<Instance *> &instances, std::vector<Light *> &lights, ViewPlane &viewPlane, Settings &settings, OSL::ShadingSystem *shadingSys)
{
    std::vector<Material *> materials;
    // default material
    materials.push_back(new Material(shadingSys));
    std::ifstream stream;
    stream.open(m_filename);
	if(stream.good()){
        int x,y;
        stream.read( reinterpret_cast<char*>( &x ), sizeof x );
        stream.read( reinterpret_cast<char*>( &y ), sizeof y );
        stream.read( reinterpret_cast<char*>( &settings.m_threads ), sizeof settings.m_threads );
        stream.read( reinterpret_cast<char*>( &settings.m_cameraBounces ), sizeof settings.m_cameraBounces );
        stream.read( reinterpret_cast<char*>( &settings.m_lightBounces ) , sizeof settings.m_lightBounces  );
        viewPlane.setResolution(x,y);
		float fov;
        stream.read( reinterpret_cast<char*>( &fov ), sizeof fov );
		viewPlane.setFov(fov);
		float focusDist;
        stream.read( reinterpret_cast<char*>( &focusDist ), sizeof focusDist );
		viewPlane.setFocusDistance(focusDist);
	}
    int type;
    while(stream.good()){
        stream.read( reinterpret_cast<char*>( &type ), sizeof type );
        if(stream.good()){
            int matId, meshId;
            Matrix4x4 transform;
            Mesh *mesh;
            Instance *instance;
            //PointLight *pointLight;
            AreaLight *areaLight;
            switch(type){
                case 0:
                    std::cout << "Material\n";
                    materials.push_back(new Material(stream, shadingSys));
                    break;
                case 1:
                    std::cout << "Mesh\n";
                    transform = matrixFromStream(stream);
                    stream.read( reinterpret_cast<char*>( &matId ), sizeof matId );
                    matId++;
                    mesh = new Mesh(stream, transform, materials[matId]);
                    objects.push_back(mesh);
                    break;
                case 2:
                    std::cout << "Mesh Instance\n";
                    transform = matrixFromStream(stream);
                    stream.read( reinterpret_cast<char*>( &matId ), sizeof matId );
                    matId++;
                    stream.read( reinterpret_cast<char*>( &meshId ), sizeof meshId );
                    instance = new Instance(matId, meshId, transform);
                    instances.push_back(instance);
                    break;
                case 3:
                    std::cout << "AreaLight\n";
                    transform = matrixFromStream(stream);
                    areaLight = new AreaLight(stream,transform);
                    lights.push_back(areaLight);
                    break;
            }
        }
    }
    stream.close();
}
