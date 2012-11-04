#ifndef __OBJ_READER_H__
#define __OBJ_READER_H__
#include "../mesh.h"

class ObjReader
{
	public:
		ObjReader(){};
		Mesh *read(const char *filename);
		void readVertexLine(std::string &line, Mesh *mesh);
		void readFaceLine(std::string &line, Mesh *mesh);
	private:
};

#endif
