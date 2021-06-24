
# Store modules' files except main.cpp files
XMLPARSER_FILES := $(filter-out src/Modules/XMLParser/main.cpp, $(wildcard src/Modules/XMLParser/*.cpp))
GMLTOOBJ_FILES := $(filter-out src/Modules/GMLtoOBJ/main.cpp, $(wildcard src/Modules/GMLtoOBJ/*.cpp))
GMLCUT_FILES := $(filter-out src/Modules/GMLCut/main.cpp, $(wildcard src/Modules/GMLCut/*.cpp))
GMLSPLIT_FILES := $(filter-out src/Modules/GMLSplit/main.cpp, $(wildcard src/Modules/GMLSplit/*.cpp))

# Store modules' directory
XMLPARSER_DIR := $(wildcard src/Modules/XMLParser)
GMLTOOBJ_DIR := $(wildcard src/Modules/GMLtoOBJ)
GMLCUT_DIR := $(wildcard src/Modules/GMLCut)
GMLSPLIT_DIR := $(wildcard src/Modules/GMLSplit)

all: XMLParser GMLtoOBJ GMLCut GMLSplit CityGMLTool

# Execute 'make' in XMLPARSER_DIR
XMLParser:
	$(MAKE) -C $(XMLPARSER_DIR)

# Execute 'make' in GMLTOOBJ_DIR
GMLtoOBJ:
	$(MAKE) -C $(GMLTOOBJ_DIR)

# Execute 'make' in GMLCUT_DIR
GMLCut:
	$(MAKE) -C $(GMLCUT_DIR)

# Execute 'make' in GMLSPLIT_DIR
GMLSplit:
	$(MAKE) -C $(GMLSPLIT_DIR)

# Compile CityGMLTool with all modules
CityGMLTool: src/main.cpp src/Modules/GMLtoOBJ/* src/Modules/* src/Modules/XMLParser/* src/Modules/GMLSplit/* src/Modules/GMLCut/* src/CLI/*  src/CityModel/* src/CityGMLTool/*
	g++ src/main.cpp \
		$(GMLTOOBJ_FILES) \
		src/Modules/Module.cpp \
		$(XMLPARSER_FILES) \
		$(GMLSPLIT_FILES) \
		$(GMLCUT_FILES) \
		src/CLI/CLI.cpp \
		src/CityModel/*.cpp \
		src/CityModel/ADE/*.cpp \
		src/CityModel/ADE/document/*.cpp \
		src/CityModel/ADE/temporal/*.cpp \
		src/CityGMLTool/*.cpp \
	-o CityGMLTool \
		-I src/Modules/GMLtoOBJ \
		-I src/Modules \
		-I src/Modules/XMLParser \
		-I src/Modules/GMLSplit \
		-I src/Modules/GMLCut \
		-I src/CLI \
		-I src/CityModel \
		-I src/CityGMLTool \
		-lxml2 -I/usr/include/libxml2 \
		-lgdal -I/usr/include/gdal \
        -lGL -lGLU -lGLEW
