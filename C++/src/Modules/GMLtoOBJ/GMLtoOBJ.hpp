#ifndef GMLTOOBJ_HPP
#define GMLTOOBJ_HPP


#include <fstream>
#include <math.h>
#include <float.h>
#include "../Module.hpp"
#include "../../CityModel/CityModel.hpp"

using namespace citygml;

class GMLtoOBJ : public Module
{
public:
    GMLtoOBJ(std::string name);

	void processOutputLocation(std::string & arg);

    void createMyOBJ(const citygml::CityModel& cModel, std::string argOutputLoc);
	std::string eraseExtension(const std::string& filename);


	void processCityModel(const citygml::CityModel& cityModel);
	void processCityObject(const citygml::CityObject& cityObject);
	void processGeometries(const citygml::CityObject& cityObject);

	void setGMLFilename(const std::string & filename);
	void setLowerBoundCoord(double newX, double newY, double newZ);

private:
	void exportMaterials(const std::string& filename);

	std::ofstream file;
	std::string gmlFilename;
	std::string outputLocation;	// path to ouput location : "output/obj/<filename>" or "/path/to/<filename>"

	std::map<std::string, std::string> m_materials;

	int vertexCounter = 1;
	int texturCounter;
	double lowerBoundX = 0.0;
	double lowerBoundY = 0.0;
	double lowerBoundZ = 0.0;
};

#endif // !GMLTOOBJ_HPP
