# GMLtoOBJ

## 💡 General informations

This module can convert a **CityModel** (data structure obtained after parsing a **CityGML** file) to an [.OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file) file format.

Texture processing is supported through the creation of an [.MTL](https://fr.wikipedia.org/wiki/Material_Template_Library) file.

* **More information about OBJ file format in wiki : [OBJ file format](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/OBJ_format)**
* **More information about this module in wiki : [GMLtoOBJ](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/Module_GMLtoOBJ)**


## 🔨 Install

### Dependencies

* `Module.hpp/.cpp` base class
* [`CityModel`](../../CityModel/) obtained after parsing with [`XMLParser`](../XMLParser/) module

## 🚀 Usage

```bash

<executable> <CityGML file> --obj [OPTIONS]

```

* `<CityGML file>` : must be a CityGML file (ends with **.gml**)
* `[OPTIONS]` : 
   * you can specify a directory output, **.obj** file produced will be name after the input **.gml** file
   * you can specify a name for the **.obj** output file
   * you can specify a directory + a name (ex: `directory/name.obj`) ⚠️ **BUT all folders browsed MUST exist** ⚠️

## 💥 Known issues

N/A

**If you find any, please let us know. (Or solve it 😜)**
