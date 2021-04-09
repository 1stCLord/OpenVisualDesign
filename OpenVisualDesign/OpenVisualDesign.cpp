// OpenVisualDesign.cpp : Defines the entry point for the application.
//

#include "OpenVisualDesign.h"
#include "Header.h"
#include "UserInterface/UserInterface.h"
#include <imgui.h>

using namespace std;

OpenVisualDesign::OpenVisualDesign()
{
	headers.push_back(std::make_unique<OVD::Header>("..\\..\\..\\..\\example.h"));
}

int main()
{
	OVD::UserInterface ui;
	ui.run();
	return 0;
}
