#include "Window.h"
#include "imgui.h"

namespace OVD
{
	Window::Window(UserInterface* user_interface) :user_interface(user_interface) {}

	bool Window::cursor_in_window(ImVec2 position, ImVec2 size)
	{
		ImVec2 cursor_pos = ImGui::GetMousePos();
		if (cursor_pos.x > position.x && cursor_pos.x < (position.x + size.x))
		{
			if (cursor_pos.y > position.y && cursor_pos.y < (position.y + size.y))
			{
				return true;
			}
		}
		return false;
	}

	UserInterface* Window::get_ui() const { return user_interface; }
	UserInterface::Config& Window::conf() const { return get_ui()->config; }
}