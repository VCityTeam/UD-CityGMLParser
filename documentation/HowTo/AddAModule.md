# How to add your module

## Step 1 - Create your class

To keep things clean, it is recommended that you create a subdirectory in [Modules](../../src/Modules) and organize your code like you want in that subdirectory.

Your base class needs to inherit from the class `Modules`, like this :

```c++
#ifndef MYMODULE_HPP
#define MYMODULE_HPP

#include "../Module.hpp";

class MyModule : Module
{
    // Do your things
}

#endif
```

## Step 2 - Add your module to CityGMLTool

Inside [CityGMLTool.cpp](../../src/CityGMLTool/CityGMLTool.cpp), create a new instance of your module, **you need a name for your module for it to be found later on** :

```c++
CityGMLTool::CityGMLTool()
{
	this->modules.push_back(new XMLParser("xmlparser")); // XMLParser exemple
    	this->modules.push_back(new MyModule("modulename")); // Add your module
}
```

Then you can add a function in `CityGMLTool` in which you call and do other things with your module. You can check the `parse(...)` function inside [CityGMLTool.cpp](../../src/CityGMLTool/CityGMLTool.cpp) in order to get inspiration.

To find your module, there is function :
```c++
Module* findModuleByName(const std::string name); // Choose a name wisely...
```

You should have something like this :
```c++
void myFunction( <my parameters> )  // Pass on any paramters you want
{
    // XMLParser exemple inside the parse function
    XMLParser* xmlparser = static_cast<XMLParser*>(this->findModuleByName("xmlparser"));
    // Your module
    MyModule* mymodule = static_cast<MyModule*>(this->findModuleByName("modulename"));

    // ... do other stuff
}
```

## Step 3 - Create a CLI argument to call your module from the command line

In [CLI.cpp](../../src/CLI/CLI.cpp), in the constructor definition, add a new argument like this :

```c++
CLI::CLI(int argc, char* argv[])
{
    // ...

	_cliParams.push_back(CLIParam("--help", "Prints usage."));
	_cliParams.push_back(CLIParam("--obj", "Convert a CityGML file into OBJ file.", std::vector<bool>({ 0 })));
    
    /*  
        * The first parameter of CLIParam is the CLI argument, it is required
        * The second paramater is a description that will be print by usage() function, it is optional
        * The third paramter is a bool array which represents the number of parameter, it is optional : no bool array means no parameters
        * Inside the bool array : 1 is REQUIRED and 0 is OPTIONAL
        * /!\ IMPORTANT NOTE : be sure to place your required parameters BEFORE optional ones /!\
    */
   // Exemples with your argument
    _cliParams.push_back(CLIParam("--flag"));   // No description, no parameters
    _cliParams.push_back(CLIParam("--flag", "Description"));    // With description but no parameters
    _cliParams.push_back(CLIParam("--flag", "Description", std::vector<bool>({ 0 })));  // Description, with 1 parameter which is optional (0)
    _cliParams.push_back(CLIParam("--flag", "Description", std::vector<bool>({ 1, 1, 0 })));    // Description, with 3 parameters : 2 required (1) and 1 optional (0)

}
```

Finally, locate the `processCmdLine()` function in [CLI.cpp](../../src/CLI/CLI.cpp) file, and add an if statement then call your function implemented earlier :

```c++
void CLI::processCmdLine()
{
	// ...

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
				
			} 
		        // ADD THIS
		        else if (name == "--flag") { // Your flag argument here
				// Be sure to call your function implemented in CityGMLTool.cpp
				_citygmltool->myFunction( <my parameters> );
		    	}
		}
	}
}
```
