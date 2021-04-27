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

	class ExecutionLane;
	class CalleePanel : public Window
	{
	public:
		CalleePanel(UserInterface* user_interface, ExecutionLane const* parent, Definition::Callee* callee, ImVec2 size, int index, bool has_popup) : Window(user_interface), parent(parent), callee(callee), size(size), index(index), has_popup(has_popup) {}

		void render() override;
		void render_panel_core(bool is_dragdrop);
		void render_dragdrop();
		void render_dragdrop_return();
		void render_grab_handle();
		void drop_param(uint8_t index);
		void render_param_inputs();

		PanelLocations locations;
	private:
		ExecutionLane const* parent;
		Definition::Callee* callee;
		ImVec2 size;
		int index;
		bool has_popup;
	};
}