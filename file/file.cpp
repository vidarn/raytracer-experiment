#include "file.h"
#include "../geom/sphere.h"
#include "../geom/mesh.h"
#include <cstring>
#include <vector>

File::File(const char *filename)
{
    m_filename = new char[strlen(filename)+1];
    strcpy(m_filename,filename);
    m_filename[strlen(filename)] = '\0';
}

void File::read(std::vector<GeometricObject *> &objects, std::vector<Light *> &lights)
{
    std::ifstream stream;
    stream.open(m_filename);
    int type;
    while(stream.good()){
        stream.read( reinterpret_cast<char*>( &type ), sizeof type );
        if(stream.good()){
            Matrix4x4 transform(stream);
            RGBA col = RGBA(0.8,0.2,0.1,1.0);
            Material *mat = new Material(col);
            Sphere *sphere;
            Mesh *mesh;
            PointLight *pointLight;
            switch(type){
                case 1:
                    std::cout << "Sphere!\n";
                    sphere = new Sphere(stream,transform,mat);
                    objects.push_back(sphere);
                    break;
                case 2:
                    std::cout << "Mesh!\n";
                    mesh = new Mesh(stream, transform, mat);
                    objects.push_back(mesh);
                    break;
                case 3:
                    std::cout << "PointLight\n";
                    pointLight = new PointLight(stream,transform);
                    lights.push_back(pointLight);
            }
        }
    }
    stream.close();
}