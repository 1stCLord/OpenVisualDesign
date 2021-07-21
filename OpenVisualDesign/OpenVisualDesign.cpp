// OpenVisualDesign.cpp : Defines the entry point for the application.
//

#include "OpenVisualDesign.h"
#include "Header.h"
#include "UserInterface/UserInterface.h"
#include <imgui.h>
#include "../ppparse/source_file.h"

using namespace std;

OpenVisualDesign::OpenVisualDesign()
{
	ppparse::source_file file("..\\..\\..\\..\\example.h");

	//headers.push_back(std::make_unique<OVD::Header>("..\\..\\..\\..\\example.h"));
}

int main()
{
	OVD::UserInterface ui;
	ui.run();
	return 0;
}
