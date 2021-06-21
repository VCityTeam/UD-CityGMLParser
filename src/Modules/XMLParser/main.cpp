#include <string.h>
#include <iostream>
#include "XMLParser.hpp"
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

    delete parser;
    delete cityModel;

    return 0;
}