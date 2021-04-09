#pragma once
#include "Window.h"

namespace OVD
{
	class CallablesWindow : public Window
	{
	public:
		CallablesWindow(UserInterface* user_interface) :Window(user_interface) {}
		void render() override;

		std::function<void(Callable const *)> notify_selected_callable;
	private:
		const Callable* selected;
	};
}