# 🎉 DA-POM-VilleUnity

This repository contains 4 modules :

<!-- ======= XMLParser ======= -->
<details>
<summary> <b> 📌 XMLParser </b> </summary>
<br>

>This module parses a **CityGML** file and produces a data structure called **CityModel** representing the data of the file, i.e. information on semantics (RoofSurface, WallSurface, ...), geometries, textures among others.

<p align="right">
  <a href="src/Modules/XMLParser#readme"> 📝 See documentation (jump to README) </a>
</p>
<hr>
</details>

<!-- ======= GMLtoOBJ ======= -->
<details>
<summary> <b> 📌 GMLtoOBJ </b> </summary>
<br>

>This module can convert a **CityModel** (data structure obtained after parsing a **CityGML** file) to an [.OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file) file format.
>
>Texture processing is supported through the creation of an [.MTL](https://fr.wikipedia.org/wiki/Material_Template_Library) file.
>
>* **More information about OBJ file format in wiki : [OBJ file format](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/OBJ_format)**
>* **More information about this module in wiki : [GMLtoOBJ](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/Module_GMLtoOBJ)**

<p align="right">
  <a href="src/Modules/GMLtoOBJ#readme"> 📝 See documentation (jump to README) </a>
</p>
<hr>
</details>

<!-- ======= GMLCut ======= -->
<details>
<summary> <b> 📌 GMLCut </b> </summary>
<br>

>This module slices CityGML data by 2 different ways :
>* Cut the **CityModel** data structure (obtained after parsing with **XMLParser**) into an **intermediate CityModel** representing the tile data, then converts it into an OBJ file with the **GMLtoOBJ module**. During the cut, buildings are assigned to the tile according to their center of gravity.
>* Cut the **input CityGML** file and produces an **output CityGML** file containing the information of the desired tile.
>
>**More information about this module in wiki : [GMLCut](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/Module_GMLCut)**

<p align="right">
  <a href="src/Modules/GMLCut#readme"> 📝 See documentation (jump to README) </a>
</p>
<hr>
</details>

<!-- ======= GMLSplit ======= -->
<details>
<summary> <b> 📌 GMLSplit </b> </summary>
<br>

>This module splits a **CityGML file** into several tiles, each tile will be an **.obj** file (with an associated **.mtl** file).
>
>It uses the [GMLCut](./src/Modules/GMLCut/) internally to cut tile by tile and produces **.obj** files with the [GMLtoOBJ](./src/Modules/GMLtoOBJ/) module.
>
>* **More information about this module in wiki : [GMLSplit](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/Module_GMLSplit)**

<p align="right">
  <a href="src/Modules/GMLSplit#readme"> 📝 See documentation (jump to README) </a>
</p>
<hr>
</details>


## ❓ HowTo

* ### [Install](./documentation/HowTo/Install.md)
* ### [Add a Module](./documentation/HowTo/AddAModule.md)

## 👷 Contributors

* #### 🎓 [POM M1 2020-2021 (Student Project)] - *From 04/02/2021 to 27/05/2021* - [:memo: Project Specification](https://github.com/VCityTeam/DA-POM-VilleUnity/blob/main/cdc_43_CHEMOUL_11500326_COMBAT_11608446.pdf)
  * **[Victor COMBAT](https://github.com/VictorCombat)**
  * **[Mickael CHEMOUL](https://github.com/MickaelCH)**

## 👮 Supervisors

* Gilles GESQUIERE (Gilles.Gesquiere@univ-lyon2.fr)
* Corentin GAUTIER (corentin.gautier@universite-lyon.fr)


## 🌟 Showcase

To see some of our results in Unity, you can jump to the [Galerie](https://github.com/VCityTeam/DA-POM-VilleUnity/wiki/Galerie) wiki page where you can find some screenshots of the city of Lyon.

We have also prepared some videos that you can find in this playlist : [Youtube Playlist](https://www.youtube.com/playlist?list=PLYhGJkXJhCpgmPOTuv7m638-VC7x7ILz_)
