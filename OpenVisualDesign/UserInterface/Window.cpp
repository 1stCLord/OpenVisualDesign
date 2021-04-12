#include "Window.h"

namespace OVD
{
	Window::Window(UserInterface* user_interface) :user_interface(user_interface) {}

	UserInterface* Window::get_ui() const { return user_interface; }
	UserInterface::Config& Window::conf() const { return get_ui()->config; }
}