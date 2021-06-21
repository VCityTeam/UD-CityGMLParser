#include "XMLParser.hpp"

XMLParser::XMLParser(std::string name) : Module(name)
{
}

CityModel * XMLParser::load(const std::string & fname, ParserParams & params)
{
	this->_filename = fname;
	params.m_basePath = fname.substr(0, fname.find_last_of('/') + 1);
	params.m_basePath.push_back('/');

	CityGMLHandlerLibXml2* handler = new CityGMLHandlerLibXml2(params);

	xmlSAXHandler sh = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	sh.startDocument = citygml::startDocument;
	sh.endDocument = citygml::endDocument;
	sh.startElement = citygml::startElement;
	sh.endElement = citygml::endElement;
	sh.characters = citygml::characters;
	sh.error = citygml::fatalError;
	sh.fatalError = citygml::fatalError;

	xmlParserInputBufferPtr inputBuffer =
		xmlParserInputBufferCreateFilename(fname.c_str(), XML_CHAR_ENCODING_NONE);

	if (inputBuffer == NULL)
	{
		std::cerr << "ERROR with file: " << fname.c_str() << std::endl;
		return 0;
	}

	xmlParserCtxtPtr context =
		xmlCreateIOParserCtxt(&sh,
			handler,
			inputBuffer->readcallback,
			inputBuffer->closecallback,
			inputBuffer->context,
			XML_CHAR_ENCODING_NONE);
	if (!context)
	{
		std::cerr << "CityGML: Unable to create LibXml2 context!" << std::endl;
		delete handler;
		return 0;
	}

	context->validate = 0;

	try
	{
		xmlParseDocument(context);
	}
	catch (...)
	{
	}

	xmlFreeParserCtxt(context);

	// InputBuffer generates memory leaks. Hopelessly tried to fix this by
	// introducing the following line but alas without succes.. Good luck!
	// xmlFreeParserInputBuffer(inputBuffer);

	CityModel* model = handler->getModel();
	delete handler;
	return model;
}
