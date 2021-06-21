#include <string.h>
#include <iostream>
#include "../Modules/XMLParser/XMLParser.hpp"
#include "../Modules/GMLtoOBJ/GMLtoOBJ.hpp"
#include "GMLCut.hpp"
#include "../../CityModel/CityModel.hpp"

/* Return true if there is a CityGML (.gml) file, false otherwise */
bool assertCityGMLFile(int argc, char* argv[])
{
    if (argc < 2) return false;

	char* toMatch = ".gml";
    int len = strlen(argv[1]);
	const char* ext = &argv[1][len-4];
    return strcmp(ext, toMatch) == 0;
}

int main(int argc, char* argv[]) 
{
    // Check if there is a CityGML (.gml) file, exit if not
    if (!assertCityGMLFile(argc, argv)) {
        std::cout << "[ERROR]:.............................:[CityGML file not found] " << std::endl;
        exit(1);
    }

    std::string filename (argv[1]);

    XMLParser * parser = new XMLParser("xmlparser");

    citygml::ParserParams params = citygml::ParserParams();
	CityModel * cityModel = parser->load(filename, params);

    // == 0 if the parsing failed, file name/location may be wrong
	if (cityModel == 0)
	{
		std::cout << "[PARSING]:.............................:[FAILED]" << std::endl;
		exit(1);
	}

	std::cout << "[PARSING]:.............................:[DONE]" << std::endl;


    // Check if there are enough arguments
    if (argc < 6) {
        std::cout << "[ERROR]:.............................:[Not enough arguments] " << std::endl;
        exit(1);
    }
    // Get arguments
    double xmin = std::stod(std::string(argv[2]));
    double ymin = std::stod(std::string(argv[3]));
    double xmax = std::stod(std::string(argv[4]));
    double ymax = std::stod(std::string(argv[5]));
    // Assign by default = true
    bool assignOrCut = true;
    if (argc == 7) {
        if (strcmp(argv[6], "CUT") == 0)
            assignOrCut = false;
    }

	GMLCut* gmlcut = new GMLCut("gmlcut");
	GMLtoOBJ* gmlToObj = new GMLtoOBJ("objconverter");

	if (assignOrCut) {
        // Assign mode
		std::vector<TextureCityGML*> texturesList;
		CityModel* tile = gmlcut->assign(cityModel, &texturesList, TVec2d(xmin, ymin), TVec2d(xmin + xmax, ymin + ymax), filename);

		// Convert to .obj only if there is at least one CityObject
		if (tile->getCityObjectsRoots().size() > 0) {
			std::string outputFolder = "cut_output_obj";
			std::string filename = outputFolder + "/" + std::to_string((int)(xmin / xmax)) + "_" + std::to_string((int)(ymin / ymax)) + ".gml";

			gmlToObj->setGMLFilename(filename);
			gmlToObj->createMyOBJ(*tile, outputFolder);
		}
	}
	else {
		gmlcut->cut(filename, xmin, ymin, xmax, ymax, "");
	}

    delete parser;
    delete cityModel;
    delete gmlToObj;
    delete gmlcut;

    return 0;
}