#include "CLI.hpp"

CLI::CLI(int argc, char* argv[])
{
    this->_argc = argc;

    for (int i = 0 ; i < argc; i++)
    {
		std::string str(argv[i]);
		this->_argv.push_back(str);
		_cmdLine.append(argv[i]);
    }

	_citygmltool = new CityGMLTool();

	_cliParams.push_back(CLIParam("--obj", "Convert a CityGML file into OBJ file.", std::vector<bool>({ 0 })));
	_cliParams.push_back(CLIParam("--cut", "Cut a CityGML file into smaller CityGML file or OBJ file.", std::vector<bool>({ 1, 1, 1, 1, 0, 0 })));
	_cliParams.push_back(CLIParam("--split", "Split a CityGML file into multiple OBJ files.", std::vector<bool>({ 1, 1, 0 })));

}

void CLI::run()
{
	// Print full command line
	for (int i = 0; i < _argc; i++)
	{
		std::cout << this->_argv.at(i) << " ";
	}
	std::cout << std::endl;

	parseCmdLine();
	processCmdLine();

	// Clean memory
	if (_citygmltool != nullptr)
		delete _citygmltool;
}

void CLI::parseCmdLine()
{
	// If no parameters : print usage
	if (this->_argc < 2)
	{
		outstream << "[ERROR]: No parameters found." << std::endl;
		usage();
	}

	// Check if there is a .gml file
	if (assertCityGMLFile())
	{
		// Check for CLI arguments
		for (int i = 0; i < _cliParams.size(); i++)
		{
			for (int j = 0; j < _argv.size(); j++)
			{
				if (_argv[j].size() == _cliParams[i]._name.size()
					&& _argv[j].compare(_argv[j].size() - _cliParams[i]._name.size(), _cliParams[i]._name.size(), _cliParams[i]._name) == 0)
				{
					_cliParams[i]._found = true;
					for (int k = 0; k < _cliParams[i]._argsRequirements.size(); k++)
					{
						j++;
						// If argument is true (REQUIRED)
						if (_cliParams[i]._argsRequirements[k])
						{
							if (j >= _argv.size())
							{
								outstream << "[ERROR]: Parsing arg: " << _cliParams[i]._name << " Argument REQUIRED not found. Exiting." << std::endl;
								usage();
							}
							else
							{
								_cliParams[i]._args.push_back(_argv[j]);
							}
						}
						// If argument is false (OPTIONAL)
						else
						{
							if (j < _argv.size())
							{
								_cliParams[i]._args.push_back(_argv[j]);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		outstream << "[ERROR]: No CityGML file found. You may want to check the location of the file. Exiting." << std::endl;
		usage();
	}
}

void CLI::processCmdLine()
{
	// Parse the CityGML file
	_citygmltool->parse(_gmlFilename);

	// Process found arguments
	for (int i = 0; i < _cliParams.size(); i++)
	{
		if (_cliParams[i]._found)
		{
			std::string name = _cliParams[i]._name;
			if (name == "--help") {
				usage();
			}
			else if (name == "--debug") {
				std::cout << "[NOT IMPLEMENTED YET] --debug" << std::endl;
			}
			else if (name == "--obj") {
				// Is there an optional parameter ?
				if (_cliParams[i]._args.size() > 0) {
					std::string arg = _cliParams[i]._args[0];
					_citygmltool->createOBJ(_gmlFilename, arg);
				}
				else {
					// No optional parameter found
					_citygmltool->createOBJ(_gmlFilename);
				}
				
			}
			else if (name == "--cut") {
				//TODO: handle optional parameter (output location)

				_citygmltool->gmlCut(
					_gmlFilename,
					std::stod(_cliParams[i]._args[0]),
					std::stod(_cliParams[i]._args[1]),
					std::stod(_cliParams[i]._args[2]),
					std::stod(_cliParams[i]._args[3]),
					(_cliParams[i]._args.size() > 4) ?		// if optional "CUT" or "ASSIGN" is present
						((_cliParams[i]._args[4] == "CUT") ? false : true)
						: true
				);
			}
			else if (name == "--split") {
				//TODO: handle stoi exception with invalid argument
				//TODO: handle optional output parameter
				_citygmltool->gmlSplit(
					_gmlFilename,
					std::stoi(_cliParams[i]._args[0]),		// tileX
					std::stoi(_cliParams[i]._args[1])		// tileY
				);
			}
		}
	}
}

bool CLI::assertCityGMLFile()
{
	std::string toMatch = ".gml";
	if (_argv[1].size() >= toMatch.size() && _argv[1].compare(_argv[1].size() - toMatch.size(), toMatch.size(), toMatch) == 0)
	{
		this->_gmlFilename = this->_argv[1];

		return true;
	}

	return false;
}

void CLI::usage()
{
	std::cout << outstream.str() << std::endl << std::endl;

	std::cout << "Usage: " << std::endl;
	std::cout << "\t citygmltool <gitygmlfile> [options]" << std::endl;

	std::cout << "[options]: " << std::endl;

	for (int i = 0; i < _cliParams.size(); i++)
	{
		_cliParams[i].print();
	}

	exit(1);
}