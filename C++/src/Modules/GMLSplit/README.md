# GMLSplit

## 💡 General informations

This module splits a **CityGML file** into several tiles, each tile will be an **.obj** file (with an associated **.mtl** file).

It uses the [GMLCut](../GMLCut/) internally to cut tile by tile and produces **.obj** files with the [GMLtoOBJ](../GMLtoOBJ/) module.

* **More information about this module in wiki : [GMLSplit](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/Module_GMLSplit)**

## 🔨 Install

### Dependencies

* `Module.hpp/.cpp` base class
* [`CityModel`](../../CityModel/) obtained after parsing with [`XMLParser`](../XMLParser/) module
* [`GMLCut`](../GMLCut/) module, used for cutting single tile
* [`GMLtoOBJ`](../GMLtoOBJ/) module, used to produces **.obj** for every tile

## 🚀 Usage

```bash

<executable> <CityGML file> --split [tileX] [tileY]

```

* `<CityGML file>` : must be a CityGML file (ends with **.gml**)
* `[tileX]` : size along the X axis of every tile
* `[tileY]` : size along the Y axis of every tile

## 💥 Known issues

N/A

**If you find any, please let us know. (Or solve it 😜)**

