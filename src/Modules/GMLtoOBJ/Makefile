XMLPARSER_FILES := $(filter-out ../XMLParser/main.cpp, $(wildcard ../XMLParser/*.cpp))

GMLtoOBJ: ./* ../* ../XMLParser/* ../../CityModel/*
	g++ ./*.cpp \
		../Module.cpp \
		$(XMLPARSER_FILES) \
		../../CityModel/*.cpp \
		../../CityModel/ADE/*.cpp \
		../../CityModel/ADE/document/*.cpp \
		../../CityModel/ADE/temporal/*.cpp \
	-o GMLtoOBJ \
		-I ../ \
		-I ./ \
		-I ../XMLParser \
		-I ../../CityModel \
		-lxml2 -I/usr/include/libxml2 \
		-lGL -lGLU -lGLEW
