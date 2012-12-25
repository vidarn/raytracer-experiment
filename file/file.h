#ifndef __FILE_H__
#define __FILE_H__
#include <fstream>
#include "../geom/geometricObject.h"
#include "../scene/scene.h"

class File
{
    public:
        File(const char *filename);
        void read(std::vector<GeometricObject *> &objects, std::vector<Light *> &lights, ViewPlane &viewPlane, Settings &settings);
    private:
        char *m_filename;
};

#endif
