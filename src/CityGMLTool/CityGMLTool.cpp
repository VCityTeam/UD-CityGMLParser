#include "CityGMLTool.hpp"

CityGMLTool::CityGMLTool()
{
	this->modules.push_back(new XMLParser("xmlparser"));
	this->modules.push_back(new GMLtoOBJ("objcreator"));
	this->modules.push_back(new GMLCut("gmlcut"));
	this->modules.push_back(new GMLSplit("gmlsplit"));

	// Init the GMLtoOBJ module with the global bounding box Lower Bound Coordinates (from DataProfile)
	GMLtoOBJ * gmlToObj = static_cast<GMLtoOBJ*>(this->findModuleByName("objcreator"));
	gmlToObj->setLowerBoundCoord(
		this->dataProfile.m_bboxLowerBound.x,
		this->dataProfile.m_bboxLowerBound.y,
		this->dataProfile.m_bboxLowerBound.z
	);
}

CityGMLTool::~CityGMLTool()
{
	for (int i = 0; i < this->modules.size(); i++)
	{
		delete this->modules[i];
	}

	this->modules.clear();

	delete this->cityModel;
}

Module* CityGMLTool::findModuleByName(const std::string name)
{
	for (int i = 0; i < this->modules.size(); i++)
	{
		if (this->modules[i]->getName() == name)
		{
			return this->modules[i];
		}
	}
}

void CityGMLTool::parse(std::string & filename)
{	
	XMLParser* xmlparser = static_cast<XMLParser*>(this->findModuleByName("xmlparser"));

	citygml::ParserParams params = citygml::ParserParams();
	cityModel = xmlparser->load(filename, params);

	// == 0 if the parsing failed, file name/location may be wrong
	if (cityModel == 0)
	{
		std::cout << "PARSING:.............................:[FAILED]" << std::endl;
		exit(1);
		return;
	}

	std::cout << "PARSING:.............................:[DONE]" << std::endl;
}

void CityGMLTool::createOBJ(std::string & gmlFilename, std::string output) {
	 GMLtoOBJ* mOBJconverter = static_cast<GMLtoOBJ*>(this->findModuleByName("objcreator"));

	 if(cityModel){
		mOBJconverter->setGMLFilename(gmlFilename);
	 	mOBJconverter->createMyOBJ(*cityModel, output);
	 }else {
	 	std::cout << "OBJconverter:.............................:[FAILED]: CityModel NULL" << std::endl;
	 	return;
	 }
}

void CityGMLTool::gmlCut(std::string & gmlFilename, double xmin, double ymin, double xmax, double ymax, bool assignOrCut, std::string output)
{
	GMLCut* gmlcut = static_cast<GMLCut*>(this->findModuleByName("gmlcut"));
	GMLtoOBJ* gmlToObj = static_cast<GMLtoOBJ*>(this->findModuleByName("objcreator"));

	if (assignOrCut) {
		std::vector<TextureCityGML*> texturesList;
		CityModel* tile = gmlcut->assign(this->cityModel, &texturesList, TVec2d(xmin, ymin), TVec2d(xmin + xmax, ymin + ymax), gmlFilename);

		// Convert to .obj only if there is at least one CityObject
		if (tile->getCityObjectsRoots().size() > 0) {
			std::string outputFolder = "cut_output_obj";
			std::string filename = outputFolder + "/" + std::to_string((int)(xmin / xmax)) + "_" + std::to_string((int)(ymin / ymax)) + ".gml";

			gmlToObj->setGMLFilename(filename);
			gmlToObj->createMyOBJ(*tile, outputFolder);
		}
	}
	else {
		gmlcut->cut(gmlFilename, xmin, ymin, xmax, ymax, output);
	}
}

void CityGMLTool::gmlSplit(std::string & gmlFilename, int tileX, int tileY, std::string output)
{
	GMLSplit* gmlSplit = static_cast<GMLSplit*>(this->findModuleByName("gmlsplit"));
	GMLCut* gmlcut = static_cast<GMLCut*>(this->findModuleByName("gmlcut"));
	GMLtoOBJ* gmlToObj = static_cast<GMLtoOBJ*>(this->findModuleByName("objcreator"));

	gmlSplit->split(gmlFilename, this->cityModel, gmlcut, gmlToObj, tileX, tileY, output);
}

void CityGMLTool::setFileName(std::string& filename) {
	this->filename = filename;
}
