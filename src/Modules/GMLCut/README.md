# GMLCut

## 💡 General informations

This module slices CityGML data by 2 different ways :
* Cut the **CityModel** data structure (obtained after parsing with **XMLParser**) into an **intermediate CityModel** representing the tile data, then converts it into an OBJ file with the **GMLtoOBJ module**. During the cut, buildings are assigned to the tile according to their center of gravity.
* Cut the **input CityGML** file and produces an **output CityGML** file containing the information of the desired tile.

**More information about this module in wiki : [GMLCut](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/Module_GMLCut)**

## 🔨 Install

### Dependencies

* External lib [GDAL](https://gdal.org/)
  * See **[/lib/gdal-2.0.2/](/lib/gdal-2.0.2/)** for the source files, **.dll** and **.lib** files
* `Module.hpp/.cpp` base class
* [`CityModel`](../../CityModel/) obtained after parsing with [`XMLParser`](../XMLParser/) module
* [`GMLtoOBJ`](../GMLtoOBJ/) module

## 🚀 Usage

```bash

<executable> <CityGML file> --cut [xmin] [ymin] [xmax] [ymax] [OPTIONS]

```

* `<CityGML file>` : must be a CityGML file (ends with **.gml**)
* `[xmin]` : smallest X coordinate of the desired tile
* `[ymin]` : smallest Y coordinate of the desired tile
* `[xmax]` : biggest X coordinate of the desired tile
* `[ymax]` : biggest Y coordinate of the desired tile
* `[OPTIONS]` : choose the cutting way
   * Default : parsing -> **CityModel** -> cut by assigning center of gravity -> convert to **.obj**
   * `CUT` : cut an **input CityGML file** and produces an **output CityGML file**

## 💥 Known issues

### **Default** mode

* We had some issues with compiling with **Visual Studio 2017 & 2019**
  * Error : ***xsd.exe exited with code 1***
  * ✔️ Solution (not very elegant...) : remove GDAL files causing the issue (see [the commit](https://github.com/VCityTeam/DA-POM-VilleUnity/commit/d86a1abb1c03fd78d8fc400d6f8d79eb7a79e6f3))

<br>

* Issue causing crash during runtime : ***Critical error detected c0000374***
  * It seems to happen when an allocated pointer of a GDAL class is being cleaned up
  * ✔️ Solution : replace the code that create/destroy GDAL classes
```c++

/* ..:: GDAL/OGR pointer creation ::.. */
OGRPolygon* PolyTile = new OGRPolygon; // Replace this...
OGRPolygon* PolyTile = (OGRPolygon*)OGRGeometryFactory::createGeometry(wkbPolygon); // ... by that !

/* ..:: GDAL/OGR pointer cleanup ::.. */
delete PolyTile; // Replace this...
OGRGeometryFactory::destroyGeometry(PolyTile); // ... by that !

```
*
  * Source : https://trac.osgeo.org/gdal/ticket/5045
<p align="center">
  <img src="https://github.com/VCityTeam/DA-POM-VilleUnity/blob/main/documentation/screenshots/gmlcut/prevent_error_gdal_ogr.PNG" width="700px">
</p>

*
  * Doc GDAL (createGeometry) : https://gdal.org/doxygen/classOGRGeometryFactory.html#ae4887e270d5099357f9a19b1eda6027a
<p align="center">
  <img src="https://github.com/VCityTeam/DA-POM-VilleUnity/blob/main/documentation/screenshots/gmlcut/doc_gdal_create_geometry.PNG" width="700px">
</p>

*
  * Doc GDAL (destroyGeometry) : https://gdal.org/doxygen/classOGRGeometryFactory.html#aae001086e26985d95c36ccd255a8c6d5
<p align="center">
  <img src="https://github.com/VCityTeam/DA-POM-VilleUnity/blob/main/documentation/screenshots/gmlcut/doc_gdal_destroy_geometry.PNG" width="700px">
</p>

### **CUT** mode

* Input file must have **`<gml:posList> </gml:posList>`** to represent vertices data (`<gml:pos> </gml:pos>` not supported)
* There must be NO vector representing the position of a vertex at 0, so no group of 3 coordinates inside the `<gml:posList>` must be at 0

