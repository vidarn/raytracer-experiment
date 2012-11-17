#ifndef __FILE_H__
#define __FILE_H__
#include <fstream>
#include "../geom/geometricObject.h"
#include "../scene/scene.h"

class File
{
    public:
        File(const char *filename);
        void read(std::vector<GeometricObject *> &objects);
    private:
        char *m_filename;
};

#endif
