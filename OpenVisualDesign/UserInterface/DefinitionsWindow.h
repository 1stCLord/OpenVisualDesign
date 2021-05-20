#pragma once
#include "Window.h"

namespace OVD
{
	class DefinitionsWindow : public Window
	{
	public:
		DefinitionsWindow(UserInterface* user_interface) :Window(user_interface) {}
		void render() override;
	private:
		Definition * selected;
	};
}