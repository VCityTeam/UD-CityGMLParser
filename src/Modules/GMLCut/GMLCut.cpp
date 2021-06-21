#include "GMLCut.hpp"

GMLCut::GMLCut(std::string name) : Module(name)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
* @brief Decoupe le fichier CityGML en un ensemble de tuiles dont la taille est definie en entree. Les geometries sont ici assignees a une tuile selon leur centre de gravite.
* @param model : Contient les donnees du fichier CityGML ouvert : il doit contenir un ensemble de batiments LOD2 ou du terrain
* @param texturesList : La fonction va remplir ce vector avec tous les appels de texture qu'il faudra enregistrer dans le CityGML en sortie;
* @param minTile : Coordonnee du coin bas gauche de la tuile
* @param maxTile : Coordonnee du coin haut droit de la tuile
*/
citygml::CityModel * GMLCut::assign(citygml::CityModel * model, std::vector<TextureCityGML*>* texturesList, TVec2d minTile, TVec2d maxTile, std::string pathFolder)
{
	citygml::CityModel* Tuile = new citygml::CityModel();

	OGRPolygon* PolyTile = (OGRPolygon*)OGRGeometryFactory::createGeometry(wkbPolygon);
	OGRLinearRing* RingTile = (OGRLinearRing*)OGRGeometryFactory::createGeometry(wkbLinearRing);
	RingTile->addPoint(minTile.x, minTile.y);
	RingTile->addPoint(minTile.x, maxTile.y);
	RingTile->addPoint(maxTile.x, maxTile.y);
	RingTile->addPoint(maxTile.x, minTile.y);
	RingTile->addPoint(minTile.x, minTile.y);
	PolyTile->addRing(RingTile);

	for (citygml::CityObject* obj : model->getCityObjectsRoots())
	{
		if (obj->getType() == citygml::COT_TINRelief || obj->getType() == citygml::COT_WaterBody)
		{
			std::string Name = obj->getId();
			citygml::CityObject* TIN_CO = nullptr;
			if (obj->getType() == citygml::COT_TINRelief)
				TIN_CO = new citygml::TINRelief(Name);
			else if (obj->getType() == citygml::COT_WaterBody)
				TIN_CO = new citygml::WaterBody(Name);

			citygml::Geometry* TIN = new citygml::Geometry(Name, citygml::GT_Unknown, 2);

			for (citygml::Geometry* Geometry : obj->getGeometries())
			{
				for (citygml::Polygon * PolygonCityGML : Geometry->getPolygons())
				{
					OGRLinearRing * OgrRing = (OGRLinearRing*)OGRGeometryFactory::createGeometry(wkbLinearRing);
					for (TVec3d Point : PolygonCityGML->getExteriorRing()->getVertices())
						OgrRing->addPoint(Point.x, Point.y, Point.z);

					OgrRing->closeRings();

					if (OgrRing->getNumPoints() < 4)
					{
						OGRGeometryFactory::destroyGeometry(OgrRing);
						continue;
					}

					OGRPolygon * OgrPoly = (OGRPolygon*)OGRGeometryFactory::createGeometry(wkbPolygon);
					OgrPoly->addRingDirectly(OgrRing);
					if (!OgrPoly->IsValid())
					{
						OGRGeometryFactory::destroyGeometry(OgrPoly);
						continue;
					}

					OGRPoint* Centroid = (OGRPoint*)OGRGeometryFactory::createGeometry(wkbPoint);
					OgrPoly->Centroid(Centroid);

					if (Centroid == nullptr || Centroid->getX() < minTile.x || Centroid->getX() > maxTile.x || Centroid->getY() < minTile.y || Centroid->getY() > maxTile.y) //Si le centroid n'est pas dans la tuile courante, on passe a la suivante.
					{
						OGRGeometryFactory::destroyGeometry(OgrPoly);
						OGRGeometryFactory::destroyGeometry(Centroid);
						continue;
					}
					OGRGeometryFactory::destroyGeometry(Centroid);
					OGRGeometryFactory::destroyGeometry(OgrPoly);

					std::vector<TVec2f> TexUV = PolygonCityGML->getTexCoords();

					bool HasTexture = (PolygonCityGML->getTexture() != nullptr);

					if (HasTexture && PolygonCityGML->getTexture()->getType() == "GeoreferencedTexture") //Ce sont des coordonnees georeferences qu'il faut convertir en coordonnees de texture standard
						TexUV = ConvertGeoreferencedTextures(TexUV);

					std::string Url;
					citygml::Texture::WrapMode WrapMode;
					if (HasTexture)
					{
						Url = PolygonCityGML->getTexture()->getUrl();
						WrapMode = PolygonCityGML->getTexture()->getWrapMode();
					}

					citygml::Polygon* pol = new citygml::Polygon(*PolygonCityGML);
					TIN->addPolygon(pol);

					if (HasTexture)
					{
						TexturePolygonCityGML Poly;

						Poly.Id = PolygonCityGML->getId();
						Poly.IdRing = PolygonCityGML->getExteriorRing()->getId();
						Poly.TexUV = TexUV;

						bool URLTest = false;//Permet de dire si l'URL existe deja dans TexturesList ou non. Si elle n'existe pas, il faut creer un nouveau TextureCityGML pour la stocker.
						for (TextureCityGML* Tex : *texturesList)
						{
							if (Tex->Url == Url)
							{
								URLTest = true;
								Tex->ListPolygons.push_back(Poly);
								break;
							}
						}
						if (!URLTest)
						{
							TextureCityGML* Texture = new TextureCityGML;
							Texture->Wrap = WrapMode;
							Texture->Url = Url;
							Texture->ListPolygons.push_back(Poly);
							texturesList->push_back(Texture);
						}
					}
				}
			}

			if (TIN->getPolygons().size() > 0)
			{
				TIN_CO->addGeometry(TIN);
				Tuile->addCityObject(TIN_CO);
				Tuile->addCityObjectAsRoot(TIN_CO);
			}
		}
		else if (obj->getType() == citygml::COT_Building)
		{
			std::string Name = obj->getId();
			citygml::CityObject* BuildingCO = new citygml::Building(Name);
			citygml::CityObject* RoofCO = new citygml::RoofSurface(Name + "_Roof");
			citygml::Geometry* Roof = new citygml::Geometry(Name + "_RoofGeometry", citygml::GT_Roof, 2);
			citygml::CityObject* WallCO = new citygml::WallSurface(Name + "_Wall");
			citygml::Geometry* Wall = new citygml::Geometry(Name + "_WallGeometry", citygml::GT_Wall, 2);
			citygml::CityObject* GroundCO = new citygml::GroundSurface(Name + "_Ground");
			citygml::Geometry* Ground = new citygml::Geometry(Name + "_GroundGeometry", citygml::GT_Ground, 2);

			OGRMultiPolygon* Building = (OGRMultiPolygon*)OGRGeometryFactory::createGeometry(wkbMultiPolygon);//Version OGR du batiment qui va etre remplie

			// Not very elegant ... (seen only in 'LYON_1ER_BATI_2015.gml' Building with one BuildingPart)
			if (obj->getChildren().at(0)->getType() == citygml::COT_BuildingPart) {
				obj = obj->getChildren().at(0);
			}
			for (citygml::CityObject* object : obj->getChildren())//On parcourt les objets (Wall, Roof, ...) du batiment
			{
				if (object->getType() == citygml::COT_RoofSurface)
				{
					for (citygml::Geometry* Geometry : object->getGeometries()) //pour chaque geometrie
					{
						for (citygml::Polygon * PolygonCityGML : Geometry->getPolygons()) //Pour chaque polygone
						{
							OGRLinearRing * OgrRing = (OGRLinearRing*)OGRGeometryFactory::createGeometry(wkbLinearRing);
							for (TVec3d Point : PolygonCityGML->getExteriorRing()->getVertices())
								OgrRing->addPoint(Point.x, Point.y, Point.z);

							OgrRing->closeRings();
							if (OgrRing->getNumPoints() > 3)
							{
								OGRPolygon* OgrPoly = (OGRPolygon*)OGRGeometryFactory::createGeometry(wkbPolygon);
								OgrPoly->addRingDirectly(OgrRing);
								if (OgrPoly->IsValid())
								{
									Building->addGeometryDirectly(OgrPoly);
								}
							}
							else
								OGRGeometryFactory::destroyGeometry(OgrRing);
						}
					}
				}
			}

			if (Building->IsEmpty())
				continue;

			OGRMultiPolygon * Enveloppe = GetEnveloppe(Building);

			if (Enveloppe->IsEmpty() || !Enveloppe->IsValid())
			{
				OGRGeometryFactory::destroyGeometry(Enveloppe);
				continue;
			}
			
			OGRPoint* Centroid = (OGRPoint*)OGRGeometryFactory::createGeometry(wkbPoint);
			Enveloppe->Centroid(Centroid);
			if (Centroid->getX() < minTile.x || Centroid->getX() > maxTile.x || Centroid->getY() < minTile.y || Centroid->getY() > maxTile.y) //Si le centroid n'est pas dans la tuile courante, on passe a la suivante.
			{
				OGRGeometryFactory::destroyGeometry(Enveloppe);
				OGRGeometryFactory::destroyGeometry(Centroid);
				continue;
			}
			OGRGeometryFactory::destroyGeometry(Centroid);
			OGRGeometryFactory::destroyGeometry(Enveloppe);

			for (citygml::CityObject* object : obj->getChildren())//On parcourt les objets (Wall, Roof, ...) du batiment
			{
				for (citygml::Geometry* Geometry : object->getGeometries()) //pour chaque geometrie
				{
					for (citygml::Polygon * PolygonCityGML : Geometry->getPolygons()) //Pour chaque polygone
					{
						std::vector<TVec2f> TexUV = PolygonCityGML->getTexCoords();

						bool HasTexture = (PolygonCityGML->getTexture() != nullptr);

						std::string Url;
						citygml::Texture::WrapMode WrapMode;
						if (HasTexture)
						{
							Url = PolygonCityGML->getTexture()->getUrl();
							WrapMode = PolygonCityGML->getTexture()->getWrapMode();
						}

						if (object->getType() == citygml::COT_RoofSurface) {

							citygml::Polygon* pol = new citygml::Polygon(*PolygonCityGML);
							Roof->addPolygon(pol);
						}

						else if (object->getType() == citygml::COT_WallSurface) {
							citygml::Polygon* pol = new citygml::Polygon(*PolygonCityGML);
							Wall->addPolygon(pol);
						}
						else if (object->getType() == citygml::COT_GroundSurface) {
							citygml::Polygon* pol = new citygml::Polygon(*PolygonCityGML);
							Ground->addPolygon(pol);
						}

						if (HasTexture)
						{
							TexturePolygonCityGML Poly;

							Poly.Id = PolygonCityGML->getId();
							Poly.IdRing = PolygonCityGML->getExteriorRing()->getId();
							Poly.TexUV = TexUV;

							bool URLTest = false;//Permet de dire si l'URL existe deja dans texturesList ou non. Si elle n'existe pas, il faut creer un nouveau TextureCityGML pour la stocker.
							for (TextureCityGML* Tex : *texturesList)
							{
								if (Tex->Url == Url)
								{
									URLTest = true;
									Tex->ListPolygons.push_back(Poly);
									break;
								}
							}
							if (!URLTest)
							{
								TextureCityGML* Texture = new TextureCityGML;
								Texture->Wrap = WrapMode;
								Texture->Url = Url;
								Texture->ListPolygons.push_back(Poly);
								texturesList->push_back(Texture);
							}
						}
					}
				}
			}
			bool test = false;
			if (Roof->getPolygons().size() > 0)
			{
				RoofCO->addGeometry(Roof);
				Tuile->addCityObject(RoofCO);
				BuildingCO->insertNode(RoofCO);
				test = true;
			}
			if (Wall->getPolygons().size() > 0)
			{
				WallCO->addGeometry(Wall);
				Tuile->addCityObject(WallCO);
				BuildingCO->insertNode(WallCO);
				test = true;
			}
			if (Ground->getPolygons().size() > 0)
			{
				GroundCO->addGeometry(Ground);
				Tuile->addCityObject(GroundCO);
				BuildingCO->insertNode(GroundCO);
				test = true;
			}
			if (test)
			{
				Tuile->addCityObject(BuildingCO);
				Tuile->addCityObjectAsRoot(BuildingCO);
			}
		}
		else if (obj->getType() == citygml::COT_Bridge) {
			std::string Name = obj->getId();
			citygml::CityObject* BridgeCO = new citygml::Bridge(Name);
			citygml::Geometry* BridgeGeo = new citygml::Geometry(Name + "_BridgeGeometry", citygml::GT_Unknown, 2);

			OGRMultiPolygon* Bridge = (OGRMultiPolygon*)OGRGeometryFactory::createGeometry(wkbMultiPolygon);//Version OGR du batiment qui va etre remplie

			// For Bridge node type, we go through Geometries directly
			for (citygml::Geometry* Geometry : obj->getGeometries()) //pour chaque geometrie
			{
				for (citygml::Polygon * PolygonCityGML : Geometry->getPolygons()) //Pour chaque polygone
				{
					OGRLinearRing * OgrRing = (OGRLinearRing*)OGRGeometryFactory::createGeometry(wkbLinearRing);
					for (TVec3d Point : PolygonCityGML->getExteriorRing()->getVertices())
						OgrRing->addPoint(Point.x, Point.y, Point.z);

					OgrRing->closeRings();
					if (OgrRing->getNumPoints() > 3)
					{
						OGRPolygon* OgrPoly = (OGRPolygon*)OGRGeometryFactory::createGeometry(wkbPolygon);
						OgrPoly->addRingDirectly(OgrRing);
						if (OgrPoly->IsValid())
						{
							Bridge->addGeometryDirectly(OgrPoly);
						}
					}
					else
						OGRGeometryFactory::destroyGeometry(OgrRing);
				}
			}

			if (Bridge->IsEmpty())
				continue;

			OGRMultiPolygon * Enveloppe = GetEnveloppe(Bridge);

			if (Enveloppe->IsEmpty() || !Enveloppe->IsValid())
			{
				OGRGeometryFactory::destroyGeometry(Enveloppe);
				continue;
			}

			OGRPoint* Centroid = (OGRPoint*)OGRGeometryFactory::createGeometry(wkbPoint);
			Enveloppe->Centroid(Centroid);
			if (Centroid->getX() < minTile.x || Centroid->getX() > maxTile.x || Centroid->getY() < minTile.y || Centroid->getY() > maxTile.y) //Si le centroid n'est pas dans la tuile courante, on passe a la suivante.
			{
				OGRGeometryFactory::destroyGeometry(Enveloppe);
				OGRGeometryFactory::destroyGeometry(Centroid);
				continue;
			}
			OGRGeometryFactory::destroyGeometry(Centroid);
			OGRGeometryFactory::destroyGeometry(Enveloppe);

			for (citygml::Geometry* Geometry : obj->getGeometries()) //pour chaque geometrie
			{
				for (citygml::Polygon * PolygonCityGML : Geometry->getPolygons()) //Pour chaque polygone
				{
					std::vector<TVec2f> TexUV = PolygonCityGML->getTexCoords();

					bool HasTexture = (PolygonCityGML->getTexture() != nullptr);

					std::string Url;
					citygml::Texture::WrapMode WrapMode;
					if (HasTexture)
					{
						Url = PolygonCityGML->getTexture()->getUrl();
						WrapMode = PolygonCityGML->getTexture()->getWrapMode();
					}

					citygml::Polygon* pol = new citygml::Polygon(*PolygonCityGML);
					BridgeGeo->addPolygon(pol);

					if (HasTexture)
					{
						TexturePolygonCityGML Poly;

						Poly.Id = PolygonCityGML->getId();
						Poly.IdRing = PolygonCityGML->getExteriorRing()->getId();
						Poly.TexUV = TexUV;

						bool URLTest = false;//Permet de dire si l'URL existe deja dans texturesList ou non. Si elle n'existe pas, il faut creer un nouveau TextureCityGML pour la stocker.
						for (TextureCityGML* Tex : *texturesList)
						{
							if (Tex->Url == Url)
							{
								URLTest = true;
								Tex->ListPolygons.push_back(Poly);
								break;
							}
						}
						if (!URLTest)
						{
							TextureCityGML* Texture = new TextureCityGML;
							Texture->Wrap = WrapMode;
							Texture->Url = Url;
							Texture->ListPolygons.push_back(Poly);
							texturesList->push_back(Texture);
						}
					}
				}
			}
			bool test = false;
			if (BridgeGeo->getPolygons().size() > 0)
			{
				BridgeCO->addGeometry(BridgeGeo);
				Tuile->addCityObject(BridgeCO);
				test = true;
			}
			if (test)
			{
				Tuile->addCityObject(BridgeCO);
				Tuile->addCityObjectAsRoot(BridgeCO);
			}
		}
	}

	OGRGeometryFactory::destroyGeometry(PolyTile);

	return Tuile;
}

void GMLCut::cut(std::string & filename, double xmin, double ymin, double xmax, double ymax, std::string outputLocation)
{
	std::cout << "[GML CUT MODULE]..........................[START]" << std::endl;
	std::cout << "\t [FILENAME]........................[" << filename << "]" << std::endl;
	std::cout << "\t [XMIN, YMIN] [XMAX, YMAX].........[" << xmin << ", " << ymin << "]" << " [" << xmax << ", " << ymax << "]" << std::endl;
	std::cout << "\t [OUTPUT LOCATION].................[" << outputLocation << "]" << std::endl;

	// TEMP OUTPUT LOCATION
	outputLocation = "gmlcut_out.gml";
	run(filename, xmin, ymin, xmax, ymax, outputLocation);

	std::cout << "[GML CUT MODULE]..........................[END]" << std::endl;
}

int GMLCut::run(std::string & filename, double xmin, double ymin, double xmax, double ymax, std::string outputLocation)
{
	G_xmin = xmin;
	G_xmax = xmax;
	G_ymin = ymin;
	G_ymax = ymax;

	if (VERBOSE)
		fprintf(stdout, "VERBOSE is ON\n");
	else
		fprintf(stdout, "VERBOSE is OFF\n");

	if (!((G_xmin < G_xmax) && (G_ymin < G_ymax)))
	{
		fprintf(stderr, "xmin must be < xmax AND ymin must be < ymax !\n");
		return EXIT_FAILURE;
	}

	std::string folderIN, folderOUT;
	int nbCopied = 0;
	bool POST_PROCESS_TEXTURES = false;
	xmlNodePtr appearanceMember_node = NULL;
	xmlNodePtr copy_node_appearanceMember = NULL;
	std::set<std::string> UUID_full_set;

	xmlNodePtr nodeToFindUV = NULL;
	std::map<std::string, xmlNodePtr> UUID_uv_map;

	// ---
	// 4 planes (normal and point)
	G_my4planes.n[0].x = 0;					G_my4planes.n[0].y = -1;					G_my4planes.n[0].z = 0; // bottom plane
	G_my4planes.p0[0].x = (G_xmin + G_xmax) / 2.;	G_my4planes.p0[0].y = G_ymin;				G_my4planes.p0[0].z = 0;

	G_my4planes.n[1].x = -1;					G_my4planes.n[1].y = 0;					G_my4planes.n[1].z = 0; // left plane
	G_my4planes.p0[1].x = G_xmin;				G_my4planes.p0[1].y = (G_ymin + G_ymax) / 2.;	G_my4planes.p0[1].z = 0;

	G_my4planes.n[2].x = 0;					G_my4planes.n[2].y = 1;					G_my4planes.n[2].z = 0; // up plane
	G_my4planes.p0[2].x = (G_xmin + G_xmax) / 2.;	G_my4planes.p0[2].y = G_ymax;				G_my4planes.p0[2].z = 0;

	G_my4planes.n[3].x = 1;					G_my4planes.n[3].y = 0;					G_my4planes.n[3].z = 0; // right plane
	G_my4planes.p0[3].x = G_xmax;				G_my4planes.p0[3].y = (G_ymin + G_ymax) / 2.;	G_my4planes.p0[3].z = 0;

	for (int p = 0; p < 4; p++)
		G_my4planes.n[p] = G_my4planes.n[p].normal(); // normalizing
	// ---

	xmlDocPtr out_doc = NULL;			// output document pointer
	xmlNodePtr out_root_node = NULL;	// output root node pointer

	// creates a new document
	out_doc = xmlNewDoc(BAD_CAST "1.0");

	xmlDocPtr doc;
	xmlNodePtr racine;

	// opens document
	xmlKeepBlanksDefault(0); // ignore les noeuds texte composant la mise en forme
	doc = xmlParseFile(filename.c_str());
	if (doc == NULL)
	{
		fprintf(stderr, "Invalid XML file\n");
		return EXIT_FAILURE;
	}

	// Create output file
	std::ofstream outputfile = std::ofstream(outputLocation);
	if (outputfile) {
		std::cout << "output success created" << std::endl;
		outputfile.clear();
	}
	else {
		std::cout << "output failed created" << std::endl;
		std::cout << outputLocation << std::endl;
	}

	// get root
	racine = xmlDocGetRootElement(doc);

	/*std::cout << "Name = " << racine->name << std::endl;
	std::cout << "Content = " << racine->children->content << std::endl;
	std::cout << "Properties = " << racine->properties->name << std::endl;
	std::cout << "Properties2 = " << racine->properties->children->content << std::endl;

	int a;
	std::cin >> a;*/

	if (racine == NULL)
	{
		fprintf(stderr, "Empty XML file\n");
		xmlFreeDoc(doc);
		return EXIT_FAILURE;
	}

	// parcours
	xmlNodePtr n = racine;
	if (xmlStrEqual(n->name, BAD_CAST "CityModel"))
	{
		fprintf(stdout, "%s\n", n->name);
		xmlNodePtr copy_node1 = xmlCopyNode(n, 2);
		xmlDocSetRootElement(out_doc, copy_node1);
		out_root_node = xmlDocGetRootElement(out_doc);

		if ((n->type == XML_ELEMENT_NODE) && (n->children != NULL))
		{
			for (n = n->children; n != NULL; n = n->next)
			{
				if (xmlStrEqual(n->name, BAD_CAST "cityObjectMember"))
				{
					if ((xmlStrEqual(n->children->name, BAD_CAST "Building")) || (xmlStrEqual(n->children->name, BAD_CAST "ReliefFeature"))) // ReliefFeature same principle as Building
					//if (xmlStrEqual(n->children->name, BAD_CAST "Building")) // only Building
					//if (xmlStrEqual(n->children->name, BAD_CAST "ReliefFeature")) // only ReliefFeature
					{
						double xmin_Building, ymin_Building, zmin_Building;
						double xmax_Building, ymax_Building, zmax_Building;
						xmin_Building = ymin_Building = zmin_Building = xmax_Building = ymax_Building = zmax_Building = 0.;
						std::set<std::string> UUID_set;

						if (!appearanceMember_node)
							nodeToFindUV = n->children;

						bool first = true;
						parcours_prefixe_Building_ReliefFeature_boundingbox(n->children, &GMLCut::process_Building_ReliefFeature_boundingbox, &first, &xmin_Building, &ymin_Building, &zmin_Building, &xmax_Building, &ymax_Building, &zmax_Building, &UUID_set, nodeToFindUV, &UUID_uv_map);
						//printf("\nMIN_Building: (%lf %lf %lf)\n", xmin_Building, ymin_Building, zmin_Building);
						//printf("MAX_Building: (%lf %lf %lf)\n", xmax_Building, ymax_Building, zmax_Building);

						if (!(xmax_Building < G_xmin) && !(ymax_Building < G_ymin) && !(xmin_Building > G_xmax) && !(ymin_Building > G_ymax))
						{
							if (VERBOSE)
								fprintf(stdout, "%s: %s - %s (min: %lf %lf) (max: %lf %lf)\n", n->name, n->children->name, xmlGetProp(n->children, BAD_CAST "id"), xmin_Building, ymin_Building, xmax_Building, ymax_Building);

							//xmlNs ns = { 0 }; // initialisation a zero de tous les membres
							xmlNodePtr copy_node2 = xmlCopyNode(n, 1);
							//xmlSetNs(copy_node2, &ns); //xmlSetNs(copy_node2, NULL);

							xmlAddChild(out_root_node, copy_node2);
							nbCopied++;

							if (TEXTURE_PROCESS)
							{
								for (std::set<std::string>::iterator it = UUID_set.begin(); it != UUID_set.end(); ++it)
									if (UUID_full_set.find(*it) == UUID_full_set.end())
										UUID_full_set.insert(*it);
									else
									{
										//printf("FOUND in UUID_full_set\n");
									}

								//printf("parcours_prefixe_Building_ReliefFeature_textures: %s - %s\n", n->children->name, xmlGetProp(n->children, BAD_CAST "id"));
								parcours_prefixe_Building_ReliefFeature_textures(n->children, &GMLCut::process_Building_ReliefFeature_textures, &UUID_set, folderIN, folderOUT);
							}
						}
					}
					else
					{
						if (VERBOSE)
							fprintf(stdout, " -> NOT COPIED: %s: %s\n", n->name, n->children->name);
					}
				}
				else if ((xmlStrEqual(n->name, BAD_CAST "appearanceMember")) && (TEXTURE_PROCESS == true))
				{
					// CAUTION : FOR NOW, WE SUPPOSE ONLY ONE appearanceMember
					appearanceMember_node = n;

					copy_node_appearanceMember = xmlCopyNode(appearanceMember_node, 2);
					xmlAddChild(out_root_node, copy_node_appearanceMember);

					fprintf(stdout, " -> PRE PROCESS TEXTURES (for uv coordinates) BEFORE ALL PARSING\n");
					parcours_prefixe_All_textureCoordinates(appearanceMember_node, &GMLCut::process_All_textureCoordinates, &UUID_uv_map);

					POST_PROCESS_TEXTURES = true;
					fprintf(stdout, " -> POST PROCESS TEXTURES (for texture files) AFTER ALL PARSING\n");
				}
				else
					fprintf(stdout, " -> NOT COPIED: %s\n", n->name);
			}
		}

		if (POST_PROCESS_TEXTURES)
		{
			UUID_uv_map.clear();

			fprintf(stdout, "POST PROCESS TEXTURES (for texture files) \n");

			// CAUTION : FOR NOW, WE SUPPOSE ONLY ONE appearanceMember
			process_textures(appearanceMember_node, copy_node_appearanceMember, &UUID_full_set, folderIN, folderOUT);
		}
	}

	fprintf(stdout, "--> NB COPIED: %d\n", nbCopied);

	// dumping document to file
	xmlSaveFormatFileEnc(outputLocation.c_str(), out_doc, "ISO-8859-1", 1);

	// free the documents
	xmlFreeDoc(out_doc);
	xmlFreeDoc(doc);

	return EXIT_SUCCESS;
}

void GMLCut::parcours_prefixe_Building_ReliefFeature_boundingbox(xmlNodePtr noeud, fct_process_Building_ReliefFeature_boundingbox f, bool *first, double *xmin, double *ymin, double *zmin, double *xmax, double *ymax, double *zmax, std::set<std::string> *UUID_s, xmlNodePtr b_rf, std::map<std::string, xmlNodePtr> *UUID_uvm)
{
	xmlNodePtr n;

	for (n = noeud; n != NULL; n = n->next)
	{
		(this->*f)(n, first, xmin, ymin, zmin, xmax, ymax, zmax, UUID_s, b_rf, UUID_uvm);

		if ((n->type == XML_ELEMENT_NODE) && (n->children != NULL))
		{
			parcours_prefixe_Building_ReliefFeature_boundingbox(n->children, f, first, xmin, ymin, zmin, xmax, ymax, zmax, UUID_s, b_rf, UUID_uvm);
		}
	}
}

void GMLCut::process_Building_ReliefFeature_boundingbox(xmlNodePtr noeud, bool *first_posList, double *xmin, double *ymin, double *zmin, double *xmax, double *ymax, double *zmax, std::set<std::string> *UUID_s, xmlNodePtr nodeToFindUV, std::map<std::string, xmlNodePtr> *UUID_uvm)
{
	if (noeud->type == XML_ELEMENT_NODE)
	{
		xmlChar *chemin = xmlGetNodePath(noeud);
		if (noeud->children != NULL)// && noeud->children->type == XML_TEXT_NODE) // MT
		{
			xmlChar *contenu = xmlNodeGetContent(noeud);
			if (xmlStrEqual(noeud->name, BAD_CAST "posList"))
			{
				//printf("%s -> %s\n", chemin, contenu);
				//printf("posList: %s\n", contenu);

				xmlNodePtr noeudLinearRing = noeud->parent;

				if (TEXTURE_PROCESS)
				{
					xmlNodePtr noeudPolygon = noeud->parent->parent->parent;
					xmlNodePtr noeudSurface = noeudPolygon->parent->parent;

					//printf("noeudPolygon: %s - noeudSurface: %s\n", noeudPolygon->name, noeudSurface->name);

					if (xmlGetProp(noeudPolygon, BAD_CAST "id"))
					{
						if (UUID_s->find((char *)xmlGetProp(noeudPolygon, BAD_CAST "id")) == UUID_s->end())
							UUID_s->insert((char *)xmlGetProp(noeudPolygon, BAD_CAST "id"));

						//printf("id Polygon: %s\n", xmlGetProp(noeudPolygon, BAD_CAST "id"));
					}
					else
					{
						if (UUID_s->find((char *)xmlGetProp(noeudSurface, BAD_CAST "id")) == UUID_s->end())
							UUID_s->insert((char *)xmlGetProp(noeudSurface, BAD_CAST "id"));

						//printf("id Surface: %s\n", xmlGetProp(noeudSurface, BAD_CAST "id"));
					}
				}

				// init
				char *endptr = NULL;
				bool first = true;

				double x, y, z;

				double xmin_posList, ymin_posList, zmin_posList;
				double xmax_posList, ymax_posList, zmax_posList;
				xmin_posList = ymin_posList = zmin_posList = xmax_posList = ymax_posList = zmax_posList = 0.;

				do
				{
					if (!endptr) endptr = (char *)contenu;

					x = strtod(endptr, &endptr);
					if (x != 0. && first) { xmin_posList = xmax_posList = x; } if (x != 0. && !first && x < xmin_posList) { xmin_posList = x; } if (x != 0. && !first && x > xmax_posList) { xmax_posList = x; }
					y = strtod(endptr, &endptr);
					if (y != 0. && first) { ymin_posList = ymax_posList = y; } if (y != 0. && !first && y < ymin_posList) { ymin_posList = y; } if (y != 0. && !first && y > ymax_posList) { ymax_posList = y; }
					z = strtod(endptr, &endptr);
					if (z != 0. && first) { zmin_posList = zmax_posList = z; } if (z != 0. && !first && z < zmin_posList) { zmin_posList = z; } if (z != 0. && !first && z > zmax_posList) { zmax_posList = z; }
					//printf("(%lf %lf %lf)\n", x, y, z);
					first = false;
				} while (!((x == 0.) && (y == 0.) && (z == 0.)));

				//printf("MIN_posList: (%lf %lf %lf)\n", xmin_posList, ymin_posList, zmin_posList);
				//printf("MAX_posList: (%lf %lf %lf)\n", xmax_posList, ymax_posList, zmax_posList);

				if (xmin_posList != 0. && (*first_posList)) { *xmin = xmin_posList; } if (xmin_posList != 0. && !(*first_posList) && xmin_posList < *xmin) { *xmin = xmin_posList; }
				if (ymin_posList != 0. && (*first_posList)) { *ymin = ymin_posList; } if (ymin_posList != 0. && !(*first_posList) && ymin_posList < *ymin) { *ymin = ymin_posList; }
				if (zmin_posList != 0. && (*first_posList)) { *zmin = zmin_posList; } if (zmin_posList != 0. && !(*first_posList) && zmin_posList < *zmin) { *zmin = zmin_posList; }
				if (xmax_posList != 0. && (*first_posList)) { *xmax = xmax_posList; } if (xmax_posList != 0. && !(*first_posList) && xmax_posList > *xmax) { *xmax = xmax_posList; }
				if (ymax_posList != 0. && (*first_posList)) { *ymax = ymax_posList; } if (ymax_posList != 0. && !(*first_posList) && ymax_posList > *ymax) { *ymax = ymax_posList; }
				if (zmax_posList != 0. && (*first_posList)) { *zmax = zmax_posList; } if (zmax_posList != 0. && !(*first_posList) && zmax_posList > *zmax) { *zmax = zmax_posList; }
				*first_posList = false;

				// ---
				if (!(*xmax < G_xmin) && !(*ymax < G_ymin) && !(*xmin > G_xmax) && !(*ymin > G_ymax))
				{
					xmlNodePtr noeudUV = NULL;
					xmlChar *contenuUV = NULL;
					char *endptrUV = NULL;
					if (TEXTURE_PROCESS)
					{
						char *pid = (char *)xmlGetProp(noeudLinearRing, BAD_CAST "id");
						if (pid)
						{
							if (nodeToFindUV)
								noeudUV = parcours_prefixe_Building_ReliefFeature_textureCoordinates(nodeToFindUV, &GMLCut::process_Building_ReliefFeature_textureCoordinates, noeudLinearRing);
							else
								noeudUV = (*UUID_uvm)[std::string(pid)];

							if (noeudUV)
								contenuUV = xmlNodeGetContent(noeudUV);
						}
					}

					// init
					endptr = NULL;
					first = true;

					TVec3d l0[MAX_POINTS_IN_POSLIST + 1];		// TEMP
					TVec3d l1[MAX_POINTS_IN_POSLIST + 1 + 1];	// TEMP
					TVec3d l[MAX_POINTS_IN_POSLIST];		// TEMP
					int i;

					TVec2d uv0[MAX_POINTS_IN_POSLIST + 1];	// TEMP
					TVec2d uv1[MAX_POINTS_IN_POSLIST + 1 + 1];	// TEMP
					TVec2d uv[MAX_POINTS_IN_POSLIST];		// TEMP

					do
					{
						if (!endptr) endptr = (char *)contenu; if (noeudUV) { if (!endptrUV) endptrUV = (char *)contenuUV; }
						if (first)
							i = 0;
						else
							i++;

						if (i > (MAX_POINTS_IN_POSLIST + 1))	// TEMP
						{
							fprintf(stderr, "---> STOP : PLEASE, INCREASE MAX_POINTS_IN_POSLIST IN SOURCE CODE\n");
							exit(EXIT_FAILURE);
						}

						l0[i].x = strtod(endptr, &endptr);
						l0[i].y = strtod(endptr, &endptr);
						l0[i].z = strtod(endptr, &endptr);
						//printf("p%d - %lf,%lf,%lf\n", i, l0[i].x, l0[i].y, l0[i].z);
						first = false;

						if (noeudUV)
						{
							uv0[i].x = strtod(endptrUV, &endptrUV);
							uv0[i].y = strtod(endptrUV, &endptrUV);
						}
					} while (!((l0[i].x == 0.) && (l0[i].y == 0.) && (l0[i].z == 0.)));
					i--;
					//printf("nb points: %d\n", i);

					for (int s = 0; s < i; s++)
					{
						l[s] = l0[s + 1] - l0[s];
						//l[s]=l[s].normal(); // normalizing

						//printf("s%d - p1: %lf,%lf,%lf - p2: %lf,%lf,%lf\n", s, l0[s].x, l0[s].y, l0[s].z, l0[s+1].x, l0[s+1].y, l0[s+1].z);

						if (noeudUV) { uv[s] = uv0[s + 1] - uv0[s]; }
					}

					double d;
					int j = 0;
					TVec3d l1_temp; TVec2d uv1_temp;
					bool inter, coin;

					TVec3d l1_old1; TVec2d uv1_old1;
					TVec3d l1_old2; TVec2d uv1_old2;
					TVec3d l1_save; TVec2d uv1_save;
					double G_x, G_y;
					int last_plan = -1;

					TVec3d triNormal = l[0].cross(l[1]);

					for (int s = 0; s < i; s++)
					{
						//printf("segment: %ld\n", s);
						inter = coin = false;

						for (int p = 0; p < 4; p++)
						{
							//printf("plan: %ld\n", p);

							// intersection
							if ((intersectPlane(G_my4planes.n[p], G_my4planes.p0[p], l0[s], l[s], d)) && d > 0. && d < 1.)
							{
								//printf("INTER s%d-p%d - %lf\n", s, p, d);

								if ((l0[s + 1].x >= G_xmin) && (l0[s + 1].x <= G_xmax))
									if ((l0[s + 1].y >= G_ymin) && (l0[s + 1].y <= G_ymax))
									{
										l1_temp = l0[s] + l[s] * d; if (noeudUV) { uv1_temp = uv0[s] + uv[s] * d; }
										l1_save = l1_temp; uv1_save = uv1_temp;
										if ((l1_temp.x >= G_xmin) && (l1_temp.x <= G_xmax))
											if ((l1_temp.y >= G_ymin) && (l1_temp.y <= G_ymax))
											{
												//printf(" -> KEEP INTER s+1\n");

												//printf(" -> KEEP INTER s+1 : p: %lf,%lf,%lf\n", l1_temp.x, l1_temp.y, l1_temp.z);												
												l1_old1 = l1_temp; if (noeudUV) { uv1_old1 = uv1_temp; }
												if (last_plan == -1)
													last_plan = p;
												else
													if (last_plan != p)
													{
														last_plan = -1;

														if (1)
														{
															bool GxOK, GyOK; GxOK = false; GyOK = false;
															if (l1_old2.x == G_xmin) { G_x = G_xmin; GxOK = true; /*printf("1\n");*/ }
															if (l1_old2.x == G_xmax) { G_x = G_xmax; GxOK = true; /*printf("2\n");*/ }
															if (l1_old2.y == G_ymin) { G_y = G_ymin; GyOK = true; /*printf("3\n");*/ }
															if (l1_old2.y == G_ymax) { G_y = G_ymax; GyOK = true; /*printf("4\n");*/ }

															if (l1_old1.x == G_xmin) { G_x = G_xmin; GxOK = true; /*printf("5\n");*/ }
															if (l1_old1.x == G_xmax) { G_x = G_xmax; GxOK = true; /*printf("6\n");*/ }
															if (l1_old1.y == G_ymin) { G_y = G_ymin; GyOK = true; /*printf("7\n");*/ }
															if (l1_old1.y == G_ymax) { G_y = G_ymax; GyOK = true; /*printf("8\n");*/ }

															if (GxOK == true && GyOK == true)
															{
																bool found = false;
																for (int jj = 0; jj < j; jj++)
																	if ((l1[jj].x == G_x) && (l1[jj].y == G_y))
																	{
																		found = true;
																		break;
																	}

																if (!found)
																{
																	l1_temp.x = G_x; l1_temp.y = G_y; //l1_temp.z = 170.;
																	if (calcule_Z_uv(l1_old2, l0[s], l1_old1, &l1_temp, noeudUV, uv1_old2, uv0[s], uv1_old1, &uv1_temp))
																	{
																		//printf(" -> !!!!!!!!!!!!!!! OK : DIFFERENT s+1 !!!!!!!!!!!!!!!!!!!!!\n");
																		l1[j] = l1_temp; if (noeudUV) { uv1[j] = uv1_temp; } j++;
																		l1[j] = l1_old1; if (noeudUV) { uv1[j] = uv1_old1; } j++;
																		l1[j] = l1_old2; if (noeudUV) { uv1[j] = uv1_old2; } j++;
																	}
																}
																else
																{
																	//printf(" -> !!!!!!!!!!!!!!! NOT OK : DIFFERENT s+1 !!!!!!!!!!!!!!!!!!!!!\n");
																}
															}
														}
													}

												l1_temp = l1_save; uv1_temp = uv1_save;
												if ((j == 0) || (l1[j - 1] != l1_temp))
												{
													l1[j] = l1_temp; if (noeudUV) { uv1[j] = uv1_temp; } j++;
													inter = true;
												}
												l1_temp = l0[s + 1]; if (noeudUV) { uv1_temp = uv0[s + 1]; }
												if ((j == 0) || (l1[j - 1] != l1_temp))
												{
													l1[j] = l1_temp; if (noeudUV) { uv1[j] = uv1_temp; } j++;
													inter = true;
												}
											}
									}

								if ((l0[s].x >= G_xmin) && (l0[s].x <= G_xmax))
									if ((l0[s].y >= G_ymin) && (l0[s].y <= G_ymax))
									{
										l1_temp = l0[s] + l[s] * d; // just for test below
										if ((l1_temp.x >= G_xmin) && (l1_temp.x <= G_xmax))
											if ((l1_temp.y >= G_ymin) && (l1_temp.y <= G_ymax))
											{
												//printf(" -> KEEP INTER s\n");

												l1_temp = l0[s]; if (noeudUV) { uv1_temp = uv0[s]; }
												if ((j == 0) || (l1[j - 1] != l1_temp))
												{
													l1[j] = l1_temp; if (noeudUV) { uv1[j] = uv1_temp; } j++;
													inter = true;
												}
												l1_temp = l0[s] + l[s] * d; if (noeudUV) { uv1_temp = uv0[s] + uv[s] * d; }
												if ((j == 0) || (l1[j - 1] != l1_temp))
												{
													l1[j] = l1_temp; if (noeudUV) { uv1[j] = uv1_temp; } j++;
													inter = true;
												}

												//printf(" -> KEEP INTER s : p: %lf,%lf,%lf\n", l1_temp.x, l1_temp.y, l1_temp.z);
												l1_old2 = l1_temp; if (noeudUV) { uv1_old2 = uv1_temp; }
												if (last_plan == -1)
													last_plan = p;
												else
													if (last_plan != p)
													{
														last_plan = -1;

														if (1)
														{
															bool GxOK, GyOK; GxOK = false; GyOK = false;
															if (l1_old1.x == G_xmin) { G_x = G_xmin; GxOK = true; /*printf("1\n");*/ }
															if (l1_old1.x == G_xmax) { G_x = G_xmax; GxOK = true; /*printf("2\n");*/ }
															if (l1_old1.y == G_ymin) { G_y = G_ymin; GyOK = true; /*printf("3\n");*/ }
															if (l1_old1.y == G_ymax) { G_y = G_ymax; GyOK = true; /*printf("4\n");*/ }

															if (l1_old2.x == G_xmin) { G_x = G_xmin; GxOK = true; /*printf("5\n");*/ }
															if (l1_old2.x == G_xmax) { G_x = G_xmax; GxOK = true; /*printf("6\n");*/ }
															if (l1_old2.y == G_ymin) { G_y = G_ymin; GyOK = true; /*printf("7\n");*/ }
															if (l1_old2.y == G_ymax) { G_y = G_ymax; GyOK = true; /*printf("8\n");*/ }

															if (GxOK == true && GyOK == true)
															{
																bool found = false;
																for (int jj = 0; jj < j; jj++)
																	if ((l1[jj].x == G_x) && (l1[jj].y == G_y))
																	{
																		found = true;
																		break;
																	}

																if (!found)
																{
																	l1_temp.x = G_x; l1_temp.y = G_y; //l1_temp.z = 170.;
																	if (calcule_Z_uv(l1_old1, l0[s + 1], l1_old2, &l1_temp, noeudUV, uv1_old1, uv0[s + 1], uv1_old2, &uv1_temp))
																	{
																		//printf(" -> !!!!!!!!!!!!!!! OK : DIFFERENT s !!!!!!!!!!!!!!!!!!!!!\n");
																		l1[j] = l1_old1; if (noeudUV) { uv1[j] = uv1_old1; } j++;
																		l1[j] = l1_old2; if (noeudUV) { uv1[j] = uv1_old2; } j++;
																		l1[j] = l1_temp; if (noeudUV) { uv1[j] = uv1_temp; } j++;
																	}
																}
																else
																{
																	//printf(" -> !!!!!!!!!!!!!!! NOT OK : DIFFERENT s !!!!!!!!!!!!!!!!!!!!!\n");
																}
															}
														}
													}
											}
									}
							}
						}

						// gestion des 4 coins
						if (!inter)
						{
							bool ok = false;

							if (triNormal.z)
							{
								// new c0
								if ((l0[s].x < G_xmin) && (l0[s + 1].y < G_ymin))
								{
									l1_temp.x = G_xmin; l1_temp.y = G_ymin; //l1_temp.z = (l0[s].z + l0[s+1].z)/2.;
									ok = true;
								}
								// new c3
								else if ((l0[s + 1].x < G_xmin) && (l0[s].y > G_ymax))
								{
									l1_temp.x = G_xmin; l1_temp.y = G_ymax; //l1_temp.z = (l0[s].z + l0[s+1].z)/2.;
									ok = true;
								}
								// new c1
								else if ((l0[s + 1].x > G_xmax) && (l0[s].y < G_ymin))
								{
									l1_temp.x = G_xmax; l1_temp.y = G_ymin; //l1_temp.z = (l0[s+1].z + l0[s].z)/2.;
									ok = true;
								}
								// new c2
								else if ((l0[s].x > G_xmax) && (l0[s + 1].y > G_ymax))
								{
									l1_temp.x = G_xmax; l1_temp.y = G_ymax; //l1_temp.z = (l0[s+1].z + l0[s].z)/2.;
									ok = true;
								}
							}

							if (ok)
							{
								if ((j == 0) || (l1[j - 1] != l1_temp))
								{
									// pas utile (?) de s'assurer que M est bien dans ABC car on teste la valeur de Z...
									if (calcule_Z_uv(l0[s], l0[s + 1], l0[s + 2], &l1_temp, noeudUV, uv0[s], uv0[s + 1], uv0[s + 2], &uv1_temp))
									{
										l1[j] = l1_temp; if (noeudUV) { uv1[j] = uv1_temp; } j++;
										coin = true;

										if (i > 3)
											//fprintf(stderr, "--> WARNING : noeudLinearRing: %lu, COIN WITH (i > 3)!\n", (unsigned long)noeudLinearRing); // TEMP
											fprintf(stderr, "--> WARNING : COIN WITH (i > 3)!\n"); // TEMP
									}
								}
							}
						}

						if ((!inter) && (!coin))
						{
							if ((j == 0) || (l1[j - 1] != l0[s]))
							{
								l1[j] = l0[s]; if (noeudUV) { uv1[j] = uv0[s]; } j++;
							}
						}
					}

					std::string new_posList = ""; std::string new_uvList = "";
					int new_nb_points = 0;
					bool first_point = true; int first_p, last_p;
					for (int p = 0; p < j; p++)
					{
						if ((l1[p].x >= G_xmin) && (l1[p].x <= G_xmax))
							if ((l1[p].y >= G_ymin) && (l1[p].y <= G_ymax))
							{
								new_posList += std::to_string(l1[p].x); new_posList += " ";
								new_posList += std::to_string(l1[p].y); new_posList += " ";
								new_posList += std::to_string(l1[p].z); new_posList += " ";

								if (noeudUV)
								{
									new_uvList += std::to_string(uv1[p].x); new_uvList += " ";
									new_uvList += std::to_string(uv1[p].y); new_uvList += " ";
								}

								if (first_point)
								{
									first_point = false;
									first_p = p;
								}
								last_p = p;

								new_nb_points++;
							}
					}

					//std::cout << "new posList (nb points: " << new_nb_points << "): " << new_posList << std::endl;

					if (new_nb_points)
					{
						if ((l1[first_p].x == l1[last_p].x) && (l1[first_p].y == l1[last_p].y) && (l1[first_p].z == l1[last_p].z))
						{
							//std::cout << "new posList (nb points: " << new_nb_points << "): " << new_posList << std::endl;
						}
						else
						{
							new_posList += std::to_string(l1[first_p].x); new_posList += " ";
							new_posList += std::to_string(l1[first_p].y); new_posList += " ";
							new_posList += std::to_string(l1[first_p].z); new_posList += " ";

							if (noeudUV)
							{
								new_uvList += std::to_string(uv1[first_p].x); new_uvList += " ";
								new_uvList += std::to_string(uv1[first_p].y); new_uvList += " ";
							}

							new_nb_points++;
							//std::cout << "ADD ONE POINT: new posList (new nb points: " << new_nb_points << "): " << new_posList << std::endl;
						}
					}

					if (new_nb_points >= 4)
					{
						xmlNodePtr noeudTriangle = noeudLinearRing->parent->parent;
						if (TRIANGULATE_PROCESS && xmlStrEqual(noeudTriangle->name, BAD_CAST "Triangle") && ((new_nb_points - 1) > 3))
						{
							//printf("must triangulate this polygon : %s has %d points\n", xmlGetProp(noeudLinearRing, BAD_CAST "id"), (new_nb_points-1));						

							TVec5d pp;
							char *endpos = (char *)new_posList.c_str(); char *endposUV = (char *)new_uvList.c_str();
							MyVectorOfVertices vv;

							bool same = false;
							for (int ii = 0; ii < (new_nb_points - 1); ii++)
							{
								pp.x = strtod(endpos, &endpos);
								pp.y = strtod(endpos, &endpos);
								pp.z = strtod(endpos, &endpos);

								if (noeudUV)
								{
									pp.U = strtod(endposUV, &endposUV);
									pp.V = strtod(endposUV, &endposUV);
								}
								else
								{
									pp.U = 0;
									pp.V = 0;
								}

								//printf("point %2d - (%lf %lf %lf - uv: %lf %lf)\n", ii, pp.x, pp.y, pp.z, pp.U, pp.V);
								bool pfound = false;
								for (std::size_t dd = 0; dd < vv.size(); dd++)
								{
									if ((vv[dd].x == pp.x) && (vv[dd].y == pp.y) && (vv[dd].z == pp.z))
									{
										pfound = true;
										break;
									}
								}
								if (pfound == true)
								{
									same = true;
									//fprintf(stderr, "--> WARNING : noeudLinearRing: %lu, same point in polygon with %d points!\n", (unsigned long)noeudLinearRing, (new_nb_points-1)); // TEMP									
								}
								else
									vv.push_back(pp);
							}

							if (same)
							{
								/*for (int dd=0; dd<vv.size(); dd++)
									printf("point %2d - (%lf %lf %lf - uv: %lf %lf)\n", dd, vv[dd].x, vv[dd].y, vv[dd].z, vv[dd].U, vv[dd].V);*/
									//fprintf(stderr, " ----> noeudLinearRing: %lu, polygon NOW with %lu points\n", (unsigned long)noeudLinearRing, vv.size()); // TEMP	
							}

							MyVectorOfVertices result;
							Triangulate::Process(vv, result);

							int tcount = (int)(result.size() / 3);
							for (int ii = 0; ii < tcount; ii++)
							{
								const TVec5d &p1 = result[ii * 3 + 0];
								const TVec5d &p2 = result[ii * 3 + 1];
								const TVec5d &p3 = result[ii * 3 + 2];
								//if (same) fprintf(stderr, "Triangle %d => (%lf %lf %lf - uv: %lf %lf) (%lf %lf %lf - uv: %lf %lf) (%lf %lf %lf - uv: %lf %lf)\n", ii+1, p1.x,p1.y,p1.z,p1.U,p1.V, p2.x,p2.y,p2.z,p2.U,p2.V, p3.x,p3.y,p3.z,p3.U,p3.V);
								new_posList = "";
								new_posList += std::to_string(p1.x); new_posList += " ";
								new_posList += std::to_string(p1.y); new_posList += " ";
								new_posList += std::to_string(p1.z); new_posList += " ";
								new_posList += std::to_string(p2.x); new_posList += " ";
								new_posList += std::to_string(p2.y); new_posList += " ";
								new_posList += std::to_string(p2.z); new_posList += " ";
								new_posList += std::to_string(p3.x); new_posList += " ";
								new_posList += std::to_string(p3.y); new_posList += " ";
								new_posList += std::to_string(p3.z); new_posList += " ";
								new_posList += std::to_string(p1.x); new_posList += " ";
								new_posList += std::to_string(p1.y); new_posList += " ";
								new_posList += std::to_string(p1.z); new_posList += " ";

								if (noeudUV)
								{
									new_uvList = "";
									new_uvList += std::to_string(p1.U); new_uvList += " ";
									new_uvList += std::to_string(p1.V); new_uvList += " ";
									new_uvList += std::to_string(p2.U); new_uvList += " ";
									new_uvList += std::to_string(p2.V); new_uvList += " ";
									new_uvList += std::to_string(p3.U); new_uvList += " ";
									new_uvList += std::to_string(p3.V); new_uvList += " ";
									new_uvList += std::to_string(p1.U); new_uvList += " ";
									new_uvList += std::to_string(p1.V); new_uvList += " ";
								}

								xmlNodePtr copy_tr = xmlCopyNode(noeudTriangle, 1);
								if (xmlGetProp(copy_tr->children->children, BAD_CAST "id"))
									xmlSetProp(copy_tr->children->children, BAD_CAST "gml:id", BAD_CAST(std::string((char *)xmlGetProp(copy_tr->children->children, BAD_CAST "id")) + std::string("_TRIANGULATED_") + std::to_string(ii + 1)).c_str());
								xmlNodeSetContent(copy_tr->children->children->children, BAD_CAST new_posList.c_str());
								xmlAddPrevSibling(noeudTriangle, copy_tr);

								if (noeudUV)
								{
									xmlNodePtr copy_uv = xmlCopyNode(noeudUV, 1);
									if (xmlGetProp(copy_uv, BAD_CAST "ring"))
										xmlSetProp(copy_uv, BAD_CAST "ring", BAD_CAST(std::string((char *)xmlGetProp(copy_uv, BAD_CAST "ring")) + std::string("_TRIANGULATED_") + std::to_string(ii + 1)).c_str());
									xmlNodeSetContent(copy_uv, BAD_CAST new_uvList.c_str());
									xmlAddPrevSibling(noeudUV, copy_uv);
								}
							}

							xmlUnlinkNode(noeud);
							xmlFreeNode(noeud);

							if (noeudUV)
							{
								xmlUnlinkNode(noeudUV);
								xmlFreeNode(noeudUV);
							}
						}
						else
						{
							xmlNodeSetContent(noeud, BAD_CAST new_posList.c_str());

							//if ( (i+1) == new_nb_points )
							{
								//std::cout << "id LinearRing: " << xmlGetProp(noeudLinearRing, BAD_CAST "id") << ": (old nb_points==new nb_points): " << (i+1) << " - " << new_nb_points << std::endl;
								if (noeudUV)
								{
									xmlNodeSetContent(noeudUV, BAD_CAST new_uvList.c_str());
									//printf("old_uvList: %s - new_uvList: %s\n", contenuUV, new_uvList.c_str());
								}
							}
						}
					}
					else
					{
						xmlUnlinkNode(noeud);
						xmlFreeNode(noeud);

						if (noeudUV)
						{
							xmlUnlinkNode(noeudUV);
							xmlFreeNode(noeudUV);
						}
					}

					if (noeudUV)
						xmlFree(contenuUV);
				}
				else
				{
					xmlUnlinkNode(noeud);
					xmlFreeNode(noeud);

					//std::cout << "---> PolygonSurfaceOUT" << std::endl;
				}
				// ---
			}
			//printf("\n");
			xmlFree(contenu);
		}
		else
		{
			printf("%s\n", chemin);
		}
		xmlFree(chemin);
	}
}

xmlNodePtr GMLCut::parcours_prefixe_Building_ReliefFeature_textureCoordinates(xmlNodePtr noeud, fct_process_Building_ReliefFeature_textureCoordinates f, xmlNodePtr noeud_id)
{
	xmlNodePtr n;
	xmlNodePtr noeud_find;

	for (n = noeud; n != NULL; n = n->next)
	{
		noeud_find = (this->*f)(n, noeud_id);
		if (noeud_find)
			return noeud_find;

		if ((n->type == XML_ELEMENT_NODE) && (n->children != NULL))
		{
			noeud_find = parcours_prefixe_Building_ReliefFeature_textureCoordinates(n->children, f, noeud_id);
			if (noeud_find)
				return noeud_find;
		}
	}

	return NULL;
}

xmlNodePtr GMLCut::process_Building_ReliefFeature_textureCoordinates(xmlNodePtr noeud, xmlNodePtr noeud_id)
{
	if (noeud->type == XML_ELEMENT_NODE)
	{
		if (noeud->children != NULL)
		{
			if (xmlStrEqual(noeud->name, BAD_CAST "textureCoordinates"))
			{
				char *p = (char *)xmlGetProp(noeud, BAD_CAST "ring");
				p++;

				char *pid = (char *)xmlGetProp(noeud_id, BAD_CAST "id");
				if (strcmp(pid, p) == 0)
				{
					return noeud;
				}
			}
		}
	}

	return NULL;
}

void GMLCut::parcours_prefixe_Building_ReliefFeature_textures(xmlNodePtr noeud, fct_process_Building_ReliefFeature_textures f, std::set<std::string> *UUID_s, std::string folderIN, std::string folderOUT)
{
	xmlNodePtr n;

	for (n = noeud; n != NULL; n = n->next)
	{
		(this->*f)(n, UUID_s, folderIN, folderOUT);

		if ((n->type == XML_ELEMENT_NODE) && (n->children != NULL))
		{
			parcours_prefixe_Building_ReliefFeature_textures(n->children, f, UUID_s, folderIN, folderOUT);
		}
	}
}

void GMLCut::process_Building_ReliefFeature_textures(xmlNodePtr noeud, std::set<std::string> *UUID_s, std::string folderIN, std::string folderOUT)
{
	if (noeud->type == XML_ELEMENT_NODE)
	{
		xmlChar *chemin = xmlGetNodePath(noeud);
		if (noeud->children != NULL)// && noeud->children->type == XML_TEXT_NODE) // MT
		{
			xmlChar *contenu = xmlNodeGetContent(noeud);
			if (xmlStrEqual(noeud->name, BAD_CAST "appearance"))
			{
				process_textures(noeud, NULL, UUID_s, folderIN, folderOUT);
			}
			xmlFree(contenu);
		}
		else
		{
			//printf("%s\n", chemin);
		}
		xmlFree(chemin);
	}
}

void GMLCut::process_textures(xmlNodePtr n, xmlNodePtr copy_node3, std::set<std::string> *UUID_s, std::string folderIN, std::string folderOUT)
{
	// CAUTION : if copy_node3 = NULL -> xml nodes not copied, just texture files are copied !
	if ((n->type == XML_ELEMENT_NODE) && (n->children != NULL))
	{
		for (n = n->children; n != NULL; n = n->next)
		{
			if (xmlStrEqual(n->name, BAD_CAST "Appearance"))
			{
				xmlNodePtr copy_node4 = xmlCopyNode(n, 2);
				if (copy_node3)
					xmlAddChild(copy_node3, copy_node4);

				if ((n->type == XML_ELEMENT_NODE) && (n->children != NULL))
				{
					for (xmlNodePtr nc = n->children; nc != NULL; nc = nc->next)
					{
						if (xmlStrEqual(nc->name, BAD_CAST "surfaceDataMember"))
						{
							xmlNodePtr copy_node5 = xmlCopyNode(nc, 2);

							// --- GeoreferencedTexture ---
							if (xmlStrEqual(nc->children->name, BAD_CAST "GeoreferencedTexture"))
							{
								xmlNodePtr copy_node6 = xmlCopyNode(nc->children, 1);
								xmlNodePtr copy_node_imageURI = nullptr;

								if ((nc->children->type == XML_ELEMENT_NODE) && (nc->children->children != NULL))
								{
									bool node_copied = false;

									for (xmlNodePtr nc2 = nc->children->children; nc2 != NULL; nc2 = nc2->next)
									{
										if (xmlStrEqual(nc2->name, BAD_CAST "target"))
										{
											xmlChar *contenu = xmlNodeGetContent(nc2);
											char *p = (char *)contenu;
											p++;

											if (UUID_s->find(p) != UUID_s->end())
											{
												if (copy_node3)
												{
													xmlAddChild(copy_node4, copy_node5);
													xmlAddChild(copy_node5, copy_node6);

													if (VERBOSE)
														fprintf(stdout, "GeoreferencedTexture target COPIED: %s\n", p);
												}

												node_copied = true;
											}

											xmlFree(contenu);
										}
										else if (xmlStrEqual(nc2->name, BAD_CAST "imageURI"))
											copy_node_imageURI = nc2;
									}

									if (node_copied)
										copy_texture_files(copy_node_imageURI, folderIN, folderOUT);
								}
							}
							// --- ParameterizedTexture ---
							else if (xmlStrEqual(nc->children->name, BAD_CAST "ParameterizedTexture"))
							{
								xmlNodePtr copy_node6 = xmlCopyNode(nc->children, 2);
								xmlNodePtr copy_node_imageURI, copy_node_textureType, copy_node_wrapMode, copy_node_borderColor;
								copy_node_imageURI = copy_node_textureType = copy_node_wrapMode = copy_node_borderColor = nullptr;

								if ((nc->children->type == XML_ELEMENT_NODE) && (nc->children->children != NULL))
								{
									bool first_target = true;

									for (xmlNodePtr nc2 = nc->children->children; nc2 != NULL; nc2 = nc2->next)
									{
										if (xmlStrEqual(nc2->name, BAD_CAST "target"))
										{
											xmlNodePtr copy_node_target = xmlCopyNode(nc2, 1);

											xmlChar *prop = xmlGetProp(nc2, BAD_CAST "uri");
											char *p = (char *)prop;
											p++;

											if (UUID_s->find(p) != UUID_s->end())
											{
												if (first_target)
												{
													if (copy_node3)
													{
														xmlAddChild(copy_node4, copy_node5);
														xmlAddChild(copy_node5, copy_node6);
														xmlAddChild(copy_node6, copy_node_imageURI);
													}
													copy_texture_files(copy_node_imageURI, folderIN, folderOUT);
													if (copy_node3)
													{
														xmlAddChild(copy_node6, copy_node_textureType);
														xmlAddChild(copy_node6, copy_node_wrapMode);
														xmlAddChild(copy_node6, copy_node_borderColor);
													}

													first_target = false;
												}
												if (copy_node3)
												{
													xmlAddChild(copy_node6, copy_node_target);

													if (VERBOSE)
														fprintf(stdout, "ParameterizedTexture target COPIED: %s\n", p);
												}
											}

											xmlFree(prop); // necessary ???
										}
										else if (xmlStrEqual(nc2->name, BAD_CAST "imageURI"))
											copy_node_imageURI = xmlCopyNode(nc2, 1);
										else if (xmlStrEqual(nc2->name, BAD_CAST "textureType"))
											copy_node_textureType = xmlCopyNode(nc2, 1);
										else if (xmlStrEqual(nc2->name, BAD_CAST "wrapMode"))
											copy_node_wrapMode = xmlCopyNode(nc2, 1);
										else if (xmlStrEqual(nc2->name, BAD_CAST "borderColor"))
											copy_node_borderColor = xmlCopyNode(nc2, 1);
										else
										{
											if (copy_node3)
												if (VERBOSE)
													fprintf(stdout, " -> CAUTION: ParameterizedTexture child NOT COPIED: %s\n", nc2->name);
										}
									}
								}
							}
						}
						else
						{
							if (copy_node3)
								if (VERBOSE)
									fprintf(stdout, " -> NOT COPIED: %s\n", nc->name);
						}
					}
				}
			}
			else
			{
				if (copy_node3)
					if (VERBOSE)
						fprintf(stdout, " -> NOT COPIED: %s\n", n->name);
			}
		}
	}
}

void GMLCut::copy_texture_files(xmlNodePtr noeud, std::string folderIN, std::string folderOUT)
{
	//xmlChar *imageURI = xmlNodeGetContent(noeud);

	//QString textIN, textOUT;
	//textIN = QString::fromStdString(folderIN) + "/" + QString::fromStdString(std::string((char *)imageURI));
	//textOUT = QString::fromStdString(folderOUT) + "/" + QString::fromStdString(std::string((char *)imageURI));

	//QFileInfo fiOUT(textOUT);
	//QDir dir(fiOUT.absolutePath());
	//if (!dir.exists())
	//	dir.mkpath(".");

	////std::cout << " -> textIN: " << textIN.toStdString() << std::endl;
	//QFile::copy(textIN, textOUT);
	////printf(" -> texture COPIED: %s\n", (char *) imageURI);
	////std::cout << " -> textOUT: " << textOUT.toStdString() << std::endl;

	//QFileInfo fiIN(textIN);
	//textIN = fiIN.absolutePath() + "/" + fiIN.baseName() + "." + fiIN.suffix().at(0) + fiIN.suffix().at(2) + "w";
	//QFile filew(textIN);
	//if (filew.exists())
	//{
	//	textOUT = fiOUT.absolutePath() + "/" + fiOUT.baseName() + "." + fiOUT.suffix().at(0) + fiOUT.suffix().at(2) + "w";
	//	QFile::copy(textIN, textOUT);
	//	//std::cout << " -> textOUTwf: " << textOUT.toStdString() << std::endl;
	//}

	//xmlFree(imageURI);
}

void GMLCut::parcours_prefixe_All_textureCoordinates(xmlNodePtr noeud, fct_process_All_textureCoordinates f, std::map<std::string, xmlNodePtr> *UUID_uvm)
{
	xmlNodePtr n;

	for (n = noeud; n != NULL; n = n->next)
	{
		(this->*f)(n, UUID_uvm);

		if ((n->type == XML_ELEMENT_NODE) && (n->children != NULL))
			parcours_prefixe_All_textureCoordinates(n->children, f, UUID_uvm);
	}
}

void GMLCut::process_All_textureCoordinates(xmlNodePtr noeud, std::map<std::string, xmlNodePtr> *UUID_uvm)
{
	if (noeud->type == XML_ELEMENT_NODE)
	{
		if (noeud->children != NULL)
		{
			if (xmlStrEqual(noeud->name, BAD_CAST "textureCoordinates"))
			{
				char *p = (char *)xmlGetProp(noeud, BAD_CAST "ring");
				p++;

				(*UUID_uvm)[std::string(p)] = noeud;
			}
		}
	}
}

// adapted from http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-plane-and-ray-disk-intersection/
bool GMLCut::intersectPlane(const TVec3d &n, const TVec3d &p0, const TVec3d& l0, const TVec3d &l, double &d)
{
	// assuming vectors are all normalized
	double denom = n.dot(l);
	if (fabs(denom) > 1e-6) // fabs add by MM-MT
	{
		TVec3d p0l0 = p0 - l0;
		d = n.dot(p0l0) / denom;
		return (d >= 0);
	}
	return false;
}

// ABC forment un triangle
// M est le point ou on fait le calcul (M est dans ABC)
bool GMLCut::calcule_Z_uv(TVec3d A, TVec3d B, TVec3d C, TVec3d *M,
	bool uv, TVec2d uvA, TVec2d uvB, TVec2d uvC, TVec2d *uvM)
{
	// On va se servir des coordonnees x et y qui sont toutes connues pour
	// determiner s et t qui nous permettrons ensuite de calculer le z du point M

	TVec3d AB = B - A;
	TVec3d AC = C - A;

	TVec2d uvAB, uvAC;

	if (uv)
	{
		uvAB = uvB - uvA;
		uvAC = uvC - uvA;
	}

	double s, t;
	t = (A.y * AB.x - A.x * AB.y + AB.y * M->x - AB.x * M->y) / (AB.y * AC.x - AB.x * AC.y);
	s = (M->x - A.x - t * AC.x) / AB.x;

	// AM = sAB + tAC
	M->z = A.z + s * AB.z + t * AC.z;

	// imaginons qu'on a les coords de textures (u,v) dans les points: ex : A.u et A.v
	if (uv)
	{
		uvM->x = uvA.x + s * uvAB.x + t * uvAC.x;
		uvM->y = uvA.y + s * uvAB.y + t * uvAC.y;
	}

	return !(is_inf_or_nan(M->z));
}

bool GMLCut::is_inf_or_nan(double x)
{
	return !(x <= DBL_MAX && x >= -DBL_MAX);
}