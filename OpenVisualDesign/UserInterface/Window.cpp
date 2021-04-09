#include "Window.h"

namespace OVD
{
	Window::Window(UserInterface* user_interface) :user_interface(user_interface) {}

	UserInterface* Window::get_ui() { return user_interface; }
	UserInterface::Config& Window::conf() { return get_ui()->config; }
}