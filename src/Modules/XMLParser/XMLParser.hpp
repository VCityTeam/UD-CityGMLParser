#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "../Module.hpp"
#include "CityGMLHandlerLibXml2.hpp"

namespace citygml
{
	static void startDocument(void *user_data)
	{
		static_cast<CityGMLHandlerLibXml2*>(user_data)->startDocument();
	}
	static void endDocument(void *user_data)
	{
		static_cast<CityGMLHandlerLibXml2*>(user_data)->endDocument();
	}

	static void startElement(void *user_data, const xmlChar *name, const xmlChar **attrs)
	{
		static_cast<CityGMLHandlerLibXml2*>(user_data)->startElement(name, attrs);
	}

	static void endElement(void *user_data, const xmlChar *name)
	{
		static_cast<CityGMLHandlerLibXml2*>(user_data)->endElement(name);
	}

	static void characters(void *user_data, const xmlChar *ch, int len)
	{
		static_cast<CityGMLHandlerLibXml2*>(user_data)->characters(ch, len);
	}

	static void fatalError(void *user_data, const char *msg, ...)
	{
		std::string error = "Parsing error!";

		va_list args;
		va_start(args, msg);
#ifdef MSVC
		size_t len = _vscprintf(msg, args) + 1;
		std::vector<char> buffer(len, '\0');
		if (_vsnprintf_s(&buffer[0], buffer.size(), len, msg, args)) error = &buffer[0];
#else
		size_t len = 1024;
		std::vector<char> buffer(len, '\0');
		if (vsnprintf(&buffer[0], buffer.size(), msg, args)) error = &buffer[0];
#endif
		va_end(args);

		static_cast<CityGMLHandlerLibXml2*>(user_data)->fatalError(error);
		throw new std::string(error);
	}
}

class XMLParser : public Module
{
public:
	XMLParser(std::string name);

	CityModel* load(const std::string& fname, ParserParams& params);

private:
	std::string _filename;
};


#endif // !XMLPARSER_HPP
