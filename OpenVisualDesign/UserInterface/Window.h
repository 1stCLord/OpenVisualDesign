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

		bool is_payload_accepted() { return get_ui()->payload_accepted; }
		void accept_payload() { get_ui()->payload_accepted = true; }

	private:
		UserInterface* user_interface;
	};
}