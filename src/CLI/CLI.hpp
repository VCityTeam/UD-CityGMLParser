#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <string>
#include <vector>
#include "../CityGMLTool/CityGMLTool.hpp"

struct CLIParam
{
	std::string _name;						// ex: --obj
	std::string _desc;						//
	std::vector<std::string> _args;			// ex: [0]: <filename>
	std::vector<bool> _argsRequirements;	// ex: [0]: false (OPTIONAL)
	bool _found = false;					// true if _name found, false otherwise

	CLIParam(std::string name, std::string desc = "", std::vector<bool> argsRequirements = std::vector<bool>())
		: _name(name), _desc(desc), _argsRequirements(argsRequirements) {}

	void print()
	{
		std::stringstream ss;
		ss << _name;
		for (int i = 0; i < _argsRequirements.size(); i++)
		{
			ss << " ";
			if (_argsRequirements[i])
				ss << "<REQUIRED>";
			else 
				ss << "<OPTIONAL>";
		}
		ss << std::endl;
		if (_desc.empty())
			ss << '\t' << "No description found." << std::endl;
		else
			ss << '\t' << _desc << std::endl;

		std::cout << ss.str();
	}
};

class CLI
{
public:
    CLI(int argc, char* argv[]);

	// main function
	void run();

	void parseCmdLine();
	void processCmdLine();

	bool assertCityGMLFile();

    void usage();
private:
	std::stringstream outstream;

    int _argc;
    std::vector<std::string> _argv;
	std::string _cmdLine;
	std::string _gmlFilename;

	std::vector<CLIParam> _cliParams;

	CityGMLTool* _citygmltool;
};

#endif