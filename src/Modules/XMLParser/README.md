# XMLParser

## 💡 General informations

This module parses a **CityGML** file and produces a data structure called **CityModel** representing the data of the file, i.e. information on semantics (RoofSurface, WallSurface, ...), geometries, textures among others.

## 🔨 Install

### Dependencies

* External lib [LibXML2](http://www.xmlsoft.org/)
  * See **[/lib/libxml2-2.9.3/](/lib/libxml2-2.9.3/)** for the source files, **.dll** and **.lib** files
* `Module.hpp/.cpp` base class
* [`CityModel`](../../CityModel/) obtained after parsing with [`XMLParser`](../XMLParser/) module

## 🚀 Usage

```bash

<executable> <CityGML file>

```

* `<CityGML file>` : must be a CityGML file (ends with **.gml**)

## 💥 Known issues

N/A

**If you find any, please let us know. (Or solve it 😜)**
