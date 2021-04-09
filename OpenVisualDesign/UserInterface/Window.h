#pragma once
#include "UserInterface.h"

namespace OVD
{
	class Window
	{
	public:
		Window(UserInterface* user_interface);
		virtual void render() = 0;

		UserInterface* get_ui();
		UserInterface::Config& conf();
	private:
		UserInterface* user_interface;
	};
}