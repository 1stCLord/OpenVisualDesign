#pragma once
#include "Window.h"

namespace OVD
{
	struct PanelLocations
	{
		ImVec2 before_panel_location;
		ImVec2 panel_location;
		ImVec2 return_location;
		ImVec2 param_location;
		ImVec2 end_panel_location;
	};

	class CalleePanel : public Window
	{
	public:
		static bool render_callee_panel(UserInterface *ui, Definition::Callee const * callee, ImVec2 size, int index, bool has_popup, PanelLocations &locations);
		static void render_panel_core(const UserInterface::Config &conf, Definition::Callee const *callable, ImVec2 size, int index, PanelLocations *locations, UserInterface *ui = nullptr);
		static void render_dragdrop(const UserInterface::Config &conf, Definition::Callee const * callable, ImVec2 size, int index);
		static void render_dragdrop_return(Definition::Callee const * value);
		static void render_grab_handle(const UserInterface::Config &conf);
		static void drop_param(UserInterface *ui);
	};
}