#ifndef MODULES_HPP
#define MODULES_HPP

#include <iostream>
#include <string>

class Module
{
public:
	Module(std::string name);

	std::string getName();

protected:
	std::string _name;

private:

};

#endif // !MODULES_HPP
