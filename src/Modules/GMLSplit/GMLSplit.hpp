#ifndef GMLSPLIT_HPP
#define GMLSPLIT_HPP

#include <iostream>
#include "../Module.hpp"
#include "../GMLCut/GMLCut.hpp"
#include "../GMLtoOBJ/GMLtoOBJ.hpp"
#include "../../CityModel/CityModel.hpp"

class GMLSplit : public Module
{
public:
	GMLSplit(std::string name);

	void split(std::string & filename, citygml::CityModel * cityModel, GMLCut * gmlCut, GMLtoOBJ * gmlToObj, int tileX, int tileY, std::string outputLocation);

private:

};

#endif // !GMLSPLIT_HPP
