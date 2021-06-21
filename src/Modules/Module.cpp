#include "Module.hpp"

Module::Module(std::string name) : _name(name)
{
}

std::string Module::getName()
{
	return this->_name;
}
