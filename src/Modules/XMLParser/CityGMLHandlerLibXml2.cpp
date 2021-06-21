/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
*
* This file is part of libcitygml library
* http://code.google.com/p/libcitygml
*
* libcitygml is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2.1 of the License, or
* (at your option) any later version.
*
* libcitygml is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*/

// This is the implementation file for LibXml2 parser

//#include "CityGMLHandlerLibXml2.hpp"
//
//void CityGMLHandlerLibXml2::startElement(const xmlChar * name, const xmlChar ** attrs)
//{
//	CityGMLHandler::startElement(wstos(name), attrs);
//}
//
//void CityGMLHandlerLibXml2::endElement(const xmlChar * name)
//{
//	CityGMLHandler::endElement(wstos(name));
//}
//
//void CityGMLHandlerLibXml2::characters(const xmlChar * chars, int length)
//{
//	for (int i = 0; i < length; i++) _buff << (char)chars[i];
//}
//
//inline std::string CityGMLHandlerLibXml2::wstos(const xmlChar * const str)
//{
//	return std::string((const char*)str);
//}
//
//std::string CityGMLHandlerLibXml2::getAttribute(void * attributes, const std::string & attname, const std::string & defvalue)
//{
//	const xmlChar **attrs = (const xmlChar**)attributes;
//	if (!attrs) return defvalue;
//	for (int i = 0; attrs[i] != 0; i += 2)
//		if (wstos(attrs[i]) == attname) return wstos(attrs[i + 1]);
//	return defvalue;
//}
