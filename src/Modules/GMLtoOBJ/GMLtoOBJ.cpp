#include "GMLtoOBJ.hpp"

GMLtoOBJ::GMLtoOBJ(std::string name) : Module(name)
{
}

void GMLtoOBJ::processOutputLocation(std::string & arg)
{
	std::string toMatch = ".obj";

	// 1. Test if arg is empty
	if (arg.empty()) {
		// Default procedure, output location : "output/obj/<filename>.obj"

		// Create output directory
		int cmdResult = system("mkdir output");
		if (cmdResult == 0) {
			//std::cout << "[DIRECTORY CREATED] : 'output'." << std::endl;
		}

		// Create obj directory
		cmdResult = system("cd output && mkdir obj");
		if (cmdResult == 0) {
			//std::cout << "[DIRECTORY CREATED] : 'output/obj'." << std::endl;

			outputLocation = "output/obj/" + eraseExtension(this->gmlFilename) + ".obj";
		}
		else { // already exist
			outputLocation = "output/obj/" + eraseExtension(this->gmlFilename) + ".obj";
		}
	}
	// 2. Test if arg is filename (.obj)
	else if (arg.size() >= toMatch.size() && arg.compare(arg.size() - toMatch.size(), toMatch.size(), toMatch) == 0) {
		// Output location is simply the argument provided
		outputLocation = arg;
	}
	// 3. Test if it's a directory and test if it exists
	else {
		std::string cmd;
		if (arg.back() == '/' || arg.back() == '\\') {
			cmd = "mkdir " + arg.substr(0, arg.size() - 1);
		}
		else {
			cmd = "mkdir " + arg;
			arg.append("/");
		}

		std::cout << "[COMMAND]: " << cmd << std::endl;

		int cmdResult = system(cmd.c_str());
		outputLocation = arg.append(eraseExtension(this->gmlFilename).append(".obj"));
	}
}

 void GMLtoOBJ::createMyOBJ(const citygml::CityModel& cityModel, std::string argOutputLoc) {

	processOutputLocation(argOutputLoc);

	file = std::ofstream(outputLocation);
		
	if(file){
		file.clear();
		file << "# Generated OBJ object from DA-POM project 2020 " << std::endl;
		file << "# " << std::endl;
		std::string name = eraseExtension(outputLocation);
		file << "mtllib " << name << ".mtl" << std::endl << std::endl;
		file << "o " << name << std::endl << std::endl;

		vertexCounter = 1;
		texturCounter = 0;

		processCityModel(cityModel);

		file.close();
		std::string mtlOutput = outputLocation;
		exportMaterials(mtlOutput.replace(mtlOutput.end() - 3,mtlOutput.end(),"mtl"));

		std::cout << "OBJconverter:.............................:[OK]" << std::endl;
	}else {
	 	std::cout << "OBJconverter:.............................:[FAILED]: Problem with filepath: '" << outputLocation << "'" << std::endl;
		return;
	}
 }

std::string GMLtoOBJ::eraseExtension(const std::string& filename) {
	std::string res = filename;
	const size_t last_slash_idx = res.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		res.erase(0, last_slash_idx + 1);
	}

	const size_t period_idx = res.rfind('.');
	if (std::string::npos != period_idx)
	{
		res.erase(period_idx);
	}
	return res;
}



void GMLtoOBJ::processCityModel(const citygml::CityModel & cityModel)
{
	for (int childIdx = 0; childIdx < cityModel.getCityObjectsRoots().size(); childIdx++) {
		processCityObject(*cityModel.getCityObjectsRoots()[childIdx]);
	}
}

void GMLtoOBJ::processCityObject(const citygml::CityObject & cityObject)
{
	// Check if the current CityObject has any children
	if (cityObject.getChildCount() == 0) {
		// We are at the end of the tree
		// Process geometries if the CityObject has any
		if (cityObject.getGeometries().size() > 0) {
			processGeometries(cityObject);
		}
	}
	else {
		// The current CityObject has children
		// So we need to go deeper to process CityObject's children
		for (int childIdx = 0; childIdx < cityObject.getChildCount(); childIdx++) {
			processCityObject(*cityObject.getChild(childIdx));
		}
	}
}

void GMLtoOBJ::processGeometries(const citygml::CityObject & cityObject)
{
	file << "g " << cityObject.getTypeAsString() << "\n";

	for (int geoIdx = 0; geoIdx < cityObject.getGeometries().size(); geoIdx++) {
		for (int polygonIdx = 0; polygonIdx < cityObject.getGeometry(geoIdx)->getPolygons().size(); polygonIdx++) { //faces

			citygml::Polygon * poly = cityObject.getGeometry(geoIdx)->getPolygons()[polygonIdx];

			if (poly->getTexture()) {
				std::string mat = poly->getTexture()->getUrl();
				mat = mat.substr(mat.find_last_of('/') + 1);
				mat = mat.substr(0, mat.find_last_of('.'));
				file << "usemtl " << mat << "\n";
				m_materials[mat] = poly->getTexture()->getUrl(); // add material to map, will be used by exportMaterials
			}

			int size = poly->getVertices().size();
			for (const TVec3d& v : poly->getVertices())
			{
				file << std::fixed << "v " << v.y - lowerBoundY << " " << v.z - lowerBoundZ << " " << v.x - lowerBoundX << "\n";
			}
			for (const TVec3f& vn : poly->getNormals())
			{
				file << "vn " << vn.x << " " << vn.y << " " << vn.z << "\n";
			}

			for (const TVec2f& vt : poly->getTexCoords())
			{
				file << "vt " << vt.x << " " << vt.y << "\n";
			}

			if (size != 0) {
				for (int ind = 0; ind < poly->getIndices().size(); ind += 3) { 
					file << "f " << vertexCounter + poly->getIndices()[ind + 0] << "/" << vertexCounter + poly->getIndices()[ind + 0] << "/" << vertexCounter + poly->getIndices()[ind + 0]
						 << " "  << vertexCounter + poly->getIndices()[ind + 1] << "/" << vertexCounter + poly->getIndices()[ind + 1] << "/" << vertexCounter + poly->getIndices()[ind + 1]
						 << " "  << vertexCounter + poly->getIndices()[ind + 2] << "/" << vertexCounter + poly->getIndices()[ind + 2] << "/" << vertexCounter + poly->getIndices()[ind + 2]
						 << "\n\n";

				}
			}

			vertexCounter += poly->getVertices().size();
		}
	}
}

void GMLtoOBJ::setGMLFilename(const std::string & filename)
{
	this->gmlFilename = filename;
}

void GMLtoOBJ::exportMaterials(const std::string& filename)
{
	std::cout << "MTL filename =>" << filename << std::endl;
	std::ofstream mat(filename);

	mat << "# Generated MTL object from DA-POM project 2020 " << std::endl;
	mat << "# " << std::endl << std::endl;;

	int i = 0;
	for (auto& it : m_materials)
	{
		++i;
		mat << "newmtl " << it.first << "\n";
		mat << "Ka 1.000000 1.000000 1.000000\n";
		mat << "Kd 1.000000 1.000000 1.000000\n";
		mat << "Ks 0.000000 0.000000 0.000000\n";
		mat << "Tr 1.000000\n";
		mat << "illum 1\n";
		mat << "Ns 0.000000\n";
		mat << "map_Kd " << it.second << "\n\n";
	}
	if (i == 0)//mat est vide, il faut cependant ecrire dans le mtl sinon Assimp ne peut ouvrir le fichier
	{
		mat << "newmtl No Material \n";
		mat << "Ka 1.000000 1.000000 1.000000\n";
		mat << "Kd 1.000000 1.000000 1.000000\n";
		mat << "Ks 0.000000 0.000000 0.000000\n";
		mat << "Tr 1.000000\n";
		mat << "illum 1\n";
		mat << "Ns 0.000000\n";
		mat << "map_Kd No Material \n\n";
	}
	mat.close();
}

void GMLtoOBJ::setLowerBoundCoord(double newX, double newY, double newZ)
{
	this->lowerBoundX = newX;
	this->lowerBoundY = newY;
	this->lowerBoundZ = newZ;
}
