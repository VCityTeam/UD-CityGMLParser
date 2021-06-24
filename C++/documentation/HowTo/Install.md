# Installation

## Step 1 - Clone this repository

```bash
git clone https://github.com/VCityTeam/DA-POM-VilleUnity.git
```

## Step 2 - Configuration & Installation

This document presents the complete installation of the repository, to install a specific module, refer to the documentation of the modules :
* [XMLParser](/src/Modules/XMLParser#readme)
* [GMLtoOBJ](/src/Modules/GMLtoOBJ#readme)
* [GMLCut](/src/Modules/GMLCut#readme)
* [GMLSplit](/src/Modules/GMLSplit#readme)

### Dependencies

* [LibXML2](http://www.xmlsoft.org/index.html)
* [GDAL](https://gdal.org/)

### Windows

Libraries are located in the [/lib/](/lib) folder and contain the files to be included, the **.lib** files to be linked and the **.dll** files.

Things to tell to your IDE : 
* Include directories :
  * Absolute path to `/lib/libxml2-2.9.3/include/libxml2/`
  * Absolute path to `/lib/gdal-2.0.2/include/`
* Libraries directories :
  * Absolute path to `/lib/libxml2-2.9.3/lib/`
  * Absolute path to `/lib/gdal-2.0.2/lib/`
* **.lib** to be linked :
  * `libxml2.lib`
  * `gdal_i.lib`
  * `glu32.lib`
* Paths to be added to your `Path` environment variable :
  * Absolute path to `/lib/libxml2-2.9.3/bin/`
  * Absolute path to `/lib/gdal-2.0.2/bin/`

#### With Visual Studio (tested with vs2017 & vs2019)

To start quickly with Visual Studio :
* Open Visual Studio
* (Steps are taken from [this page](https://docs.microsoft.com/en-us/cpp/build/how-to-create-a-cpp-project-from-existing-code?view=msvc-160))
* Open **File** menu > Select **New** > **Project From Existing Code**
  * Follow the Assistant steps
  * In **Projet file location**, specify the folder that you cloned the repo in
  * **Project type** : **Console application project**
  * **Debug configuration** : N/A (unless you want to)
  * **Release configuration** : N/A (unless you want to)
* Select `x64` configuration
* Then, right click on the project name > **Properties**
  * Select **VC++ Directories**
    * In **Include Directories**, add **absolute paths** to those folders :
      * `/lib/libxml2-2.9.3/include/libxml2/`
      * `/lib/gdal-2.0.2/include/`
    * In **Library Directories**, add **absolute paths** to those folders :
      * `/lib/libxml2-2.9.3/lib/`
      * `/lib/gdal-2.0.2/lib/`
  * Under **Linker**, select **Input**
    * In **Additional Dependencies**, add :
      * `libxml2.lib`
      * `gdal_i.lib`
      * `glu32.lib`
* Don't forget to add **absolute paths** to the **.dll** to your `Path`environment variable :
  * `/lib/libxml2-2.9.3/bin/`
  * `/lib/gdal-2.0.2/bin/`

### Linux

First your need to install (if not already) LibXML2 and GDAL, run :

```bash
sudo apt install libxml2-dev gdal-bin gdal-data libgdal-dev
```

Then, there is a Makefile available at the root directory, run :

#### All targets
```bash
make 
```

#### CityGMLTool
```bash
make CityGMLTool
```

#### XMLParser
```bash
make XMLParser
```

#### GMLtoOBJ
```bash
make GMLtoOBJ
```

#### GMLCut
```bash
make GMLCut
```

#### GMLSplit
```bash
make GMLSplit
```
