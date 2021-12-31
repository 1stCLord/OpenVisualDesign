// OpenVisualDesign.cpp : Defines the entry point for the application.
//

#include "OpenVisualDesign.h"
#include "Header.h"
#include "UserInterface/UserInterface.h"
#include <imgui.h>
#include <algorithm>
#include "../ppparse/source_file.h"

using namespace std;

OpenVisualDesign::OpenVisualDesign()
{
	ppparse::source_file file("..\\..\\..\\..\\example.h");
	file.print();

	//todo find all the attributes with the ovd expression, figure out if it's a class/namespace, function or variable
	//classes have the class or namespace keyword, functions have parameters and everything else is a varialbe
	file.get_elements_of_type(ppparse::node_type::attribute);
	//headers.push_back(std::make_unique<OVD::Header>("..\\..\\..\\..\\example.h"));
}

int main()
{
	OVD::UserInterface ui;
	ui.run();
	return 0;
}
