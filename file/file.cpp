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

void File::read(std::vector<GeometricObject *> &objects, std::vector<Light *> &lights, ViewPlane &viewPlane, Settings &settings)
{
    std::vector<Material *> materials;
    // default material
    materials.push_back(new Material);
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
            int mat;
            Matrix4x4 transform;
            Mesh *mesh;
            //PointLight *pointLight;
            AreaLight *areaLight;
            switch(type){
                case 0:
                    std::cout << "Material!\n";
                    materials.push_back(new Material(stream));
                    break;
                case 1:
                    std::cout << "Mesh!\n";
                    transform = Matrix4x4(stream);
                    stream.read( reinterpret_cast<char*>( &mat ), sizeof mat );
                    mat++;
                    mesh = new Mesh(stream, transform, materials[mat]);
                    objects.push_back(mesh);
                    break;
                case 2:
                    std::cout << "PointLight\n";
                    /*transform = Matrix4x4(stream);
                    pointLight = new PointLight(stream,transform);
                    lights.push_back(pointLight);*/
                    break;
                case 3:
                    std::cout << "AreaLight\n";
                    transform = Matrix4x4(stream);
                    areaLight = new AreaLight(stream,transform);
                    lights.push_back(areaLight);
                    break;
            }
        }
    }
    stream.close();
}
