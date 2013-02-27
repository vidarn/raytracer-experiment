#ifndef __FILE_H__
#define __FILE_H__
#include <fstream>
#include "../geom/geometricObject.h"
#include "../aggregates/instance.h"
#include "../scene/scene.h"
#include <OSL/oslexec.h>

class File
{
    public:
        File(const char *filename);
        void read(std::vector<GeometricObject *> &objects, std::vector<Instance *> &instances, std::vector<Light *> &lights, ViewPlane &viewPlane, Settings &settings, OSL::ShadingSystem *shadingSys);
    private:
        char *m_filename;
};

#endif
