#ifndef GMLCUT_HPP
#define GMLCUT_HPP

#include <set>
#include <map>
#include <vector>
#include <fstream>

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
# pragma warning(disable:4251) // gdal-1.11.4 internals (cpl_string.h) when
#endif                         // including ogrsf_frmts.h on VCC++
#include <ogrsf_frmts.h>

#include <libxml/tree.h>
#include "../Module.hpp"
#include "../../CityModel/CityGML.hpp"
#include "../../CityModel/CityModel.hpp"
#include "TextureCityGML.hpp"
#include "ConvertTextures.hpp"
#include "OGRGDALtools.hpp"

#include "Triangulate.hpp"

#include <float.h> // MT : for DBL_MAX on MAC OS X

struct FOUR_PLANES
{
	TVec3d n[4];
	TVec3d p0[4];
};

#define MAX_POINTS_IN_POSLIST 200

class GMLCut : public Module
{
public:
	GMLCut(std::string name);

	citygml::CityModel* assign(citygml::CityModel* model, std::vector<TextureCityGML*>* texturesList, TVec2d minTile, TVec2d maxTile, std::string pathFolder);

	void cut(std::string & filename, double xmin, double ymin, double xmax, double ymax, std::string outputLocation);

private:
	typedef void(GMLCut::*fct_process_All_textureCoordinates)(xmlNodePtr, std::map<std::string, xmlNodePtr> *);
	typedef xmlNodePtr(GMLCut::*fct_process_Building_ReliefFeature_textureCoordinates)(xmlNodePtr, xmlNodePtr);
	typedef void(GMLCut::*fct_process_Building_ReliefFeature_boundingbox)(xmlNodePtr, bool *, double *, double *, double *, double *, double *, double *, std::set<std::string> *, xmlNodePtr, std::map<std::string, xmlNodePtr> *);
	typedef void(GMLCut::*fct_process_Building_ReliefFeature_textures)(xmlNodePtr, std::set<std::string> *, std::string, std::string);


	int run(std::string & filename, double xmin, double ymin, double xmax, double ymax, std::string outputLocation);

	void parcours_prefixe_Building_ReliefFeature_boundingbox(xmlNodePtr noeud, fct_process_Building_ReliefFeature_boundingbox f, bool * first, double * xmin, double * ymin, double * zmin, double * xmax, double * ymax, double * zmax, std::set<std::string>* UUID_s, xmlNodePtr b_rf, std::map<std::string, xmlNodePtr>* UUID_uvm);

	void process_Building_ReliefFeature_boundingbox(xmlNodePtr noeud, bool * first_posList, double * xmin, double * ymin, double * zmin, double * xmax, double * ymax, double * zmax, std::set<std::string>* UUID_s, xmlNodePtr nodeToFindUV, std::map<std::string, xmlNodePtr>* UUID_uvm);

	xmlNodePtr parcours_prefixe_Building_ReliefFeature_textureCoordinates(xmlNodePtr noeud, fct_process_Building_ReliefFeature_textureCoordinates f, xmlNodePtr noeud_id);

	xmlNodePtr process_Building_ReliefFeature_textureCoordinates(xmlNodePtr noeud, xmlNodePtr noeud_id);

	void parcours_prefixe_Building_ReliefFeature_textures(xmlNodePtr noeud, fct_process_Building_ReliefFeature_textures f, std::set<std::string>* UUID_s, std::string folderIN, std::string folderOUT);

	void process_Building_ReliefFeature_textures(xmlNodePtr noeud, std::set<std::string>* UUID_s, std::string folderIN, std::string folderOUT);

	void process_textures(xmlNodePtr n, xmlNodePtr copy_node3, std::set<std::string>* UUID_s, std::string folderIN, std::string folderOUT);

	void copy_texture_files(xmlNodePtr noeud, std::string folderIN, std::string folderOUT);

	void parcours_prefixe_All_textureCoordinates(xmlNodePtr noeud, fct_process_All_textureCoordinates f, std::map<std::string, xmlNodePtr>* UUID_uvm);

	void process_All_textureCoordinates(xmlNodePtr noeud, std::map<std::string, xmlNodePtr>* UUID_uvm);

	bool intersectPlane(const TVec3d & n, const TVec3d & p0, const TVec3d & l0, const TVec3d & l, double & d);

	bool calcule_Z_uv(TVec3d A, TVec3d B, TVec3d C, TVec3d * M, bool uv, TVec2d uvA, TVec2d uvB, TVec2d uvC, TVec2d * uvM);

	bool is_inf_or_nan(double x);


	double G_xmin;
	double G_xmax;
	double G_ymin;
	double G_ymax;
	FOUR_PLANES G_my4planes;
	bool TEXTURE_PROCESS = false;
	bool TRIANGULATE_PROCESS = true;
	bool VERBOSE = true;
};

#endif // !GMLCUT_HPP
