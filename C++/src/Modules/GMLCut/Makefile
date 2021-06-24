XMLPARSER_FILES := $(filter-out ../XMLParser/main.cpp, $(wildcard ../XMLParser/*.cpp))
GMLTOOBJ_FILES := $(filter-out ../GMLtoOBJ/main.cpp, $(wildcard ../GMLtoOBJ/*.cpp))

GMLCut: ./* ../* ../XMLParser/* ../GMLtoOBJ/* ../../CityModel/*
	g++ ./*.cpp \
		../Module.cpp \
		$(XMLPARSER_FILES) \
		$(GMLTOOBJ_FILES) \
		../../CityModel/*.cpp \
		../../CityModel/ADE/*.cpp \
		../../CityModel/ADE/document/*.cpp \
		../../CityModel/ADE/temporal/*.cpp \
	-o GMLCut \
		-I ../ \
		-I ./ \
		-I ../XMLParser \
		-I ../GMLtoOBJ \
		-I ../../CityModel \
		-lxml2 -I/usr/include/libxml2 \
		-lgdal -I/usr/include/gdal \
		-lGL -lGLU -lGLEW
