#include "objReader.h"
#include <fstream>
#include <cstdlib>

Mesh *ObjReader::read(const char *filename)
{
	std::cout << "reading file \"" << filename << "\"\n";
	Mesh *mesh = new Mesh;
	std::string line;
	std::ifstream file;
	file.open(filename);
	while(file.good()){
		getline(file,line);
		switch(line[0]){
			case 'v':
			case 'V':
				readVertexLine(line,mesh);
				break;
			case 'f':
			case 'F':
				readFaceLine(line,mesh);
				break;
		}
	}
	file.close();
	mesh->calculateNormals();
	mesh->calculateBounds();
	mesh->populateCollection();
	std::cout << "obj file \"" << filename << "\" read\n";
	return mesh;
}

void ObjReader::readVertexLine(std::string &line, Mesh *mesh)
{
	float pos[3];
	int b = 0;
	for(int i=0;i<3;i++){
		int a = line.find(' ',b);
		b = line.find(' ',a+1);
		std::string tmpStr = line.substr(a,b);
		pos[i] = atof(tmpStr.c_str());
	}
	Vec3 point(pos);
	mesh->addPoint(point);
	Vec3 normal;
	mesh->addNormal(normal);
}

void ObjReader::readFaceLine(std::string &line, Mesh *mesh)
{
	int pointIds[4];
	int b = 0;
	for(int i=0;i<4 && b != -1;i++){
		int a = line.find(' ',b);
		b = line.find(' ',a+1);
		std::string tmpStr = line.substr(a,b);
		pointIds[i] = atoi(tmpStr.c_str()) - 1;
	}
	Face face = Face(mesh);
	face[0] = pointIds[0];
	face[1] = pointIds[1];
	face[2] = pointIds[2];
	mesh->addFace(face);
}
