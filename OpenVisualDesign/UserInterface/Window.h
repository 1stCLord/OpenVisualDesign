#pragma once
#include "UserInterface.h"

namespace OVD
{
	class Window
	{
	public:
		Window(UserInterface* user_interface);
		virtual void render() = 0;

		static bool cursor_in_window(ImVec2 position, ImVec2 size);

		UserInterface* get_ui() const;
		UserInterface::Config& conf() const;
	private:
		UserInterface* user_interface;
	};
}