#include "GMLSplit.hpp"

GMLSplit::GMLSplit(std::string name) : Module(name)
{
}

void GMLSplit::split(std::string & filename, citygml::CityModel * cityModel, GMLCut * gmlCut, GMLtoOBJ * gmlToObj, int tileX, int tileY, std::string outputLocation)
{
	std::cout << "[SPLIT GML FILE]...............................[START]" << std::endl;
	std::cout << "\t [GML FILENAME]....................[" << filename << "]" << std::endl;
	std::cout << "\t [OUTPUT LOCATION]....................[" << outputLocation << "]" << std::endl;

	//TODO: process output location

	TVec3d Lower = cityModel->getEnvelope().getLowerBound();
	TVec3d Upper = cityModel->getEnvelope().getUpperBound();

	TVec2d MinTile((int)(Lower.x / tileX) * tileX, (int)(Lower.y / tileY) * tileY);
	TVec2d MaxTile((int)(Upper.x / tileX) * tileX, (int)(Upper.y / tileY) * tileY);

	for (int x = (int)MinTile.x; x <= (int)MaxTile.x; x += tileX)
	{
		for (int y = (int)MinTile.y; y <= (int)MaxTile.y; y += tileY)
		{
			std::vector<TextureCityGML*> texturesList;
			citygml::CityModel* tile = gmlCut->assign(cityModel, &texturesList, TVec2d(x, y), TVec2d(x + tileX, y + tileY), filename);

			// Convert to .obj only if there is at least one CityObject
			if (tile->getCityObjectsRoots().size() > 0) {
				std::string outputFolder = "cut_output_obj";
				std::string filename = outputFolder + "/" + std::to_string((int)(x / tileX)) + "_" + std::to_string((int)(y / tileY)) + ".gml";

				gmlToObj->setGMLFilename(filename);
				gmlToObj->createMyOBJ(*tile, outputFolder);
			}
		}
	}


	std::cout << "[SPLIT GML FILE]...............................[DONE]" << std::endl;
}
