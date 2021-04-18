#pragma once
#include "Window.h"
#include "CallablesWindow.h"
#include "RegisterLane.h"
#include "CalleePanel.h"

namespace OVD
{
	class ExecutionLane : public Window
	{
	public:
		ExecutionLane(UserInterface* user_interface);
		void render() override;

		int get_column_of(Definition::Callee const* callee) const;
		float get_current_size_y_scale() const { return current_size_y_scale; }

		Definition * definition = nullptr;
		std::vector<PanelLocations> panel_locations;
	private:
		CallablesWindow callables_window;
		float current_size_y_scale = 0.8f;
		int insert_index = 0;
		std::vector<RegisterLane> register_lanes;
		
		void render_register_lanes_controls(bool render);
		void render_register_lanes(bool render);

		void render_drop_insert() const;
		void drop_callee(int destination);

		//static int accept_callee_drop(const UserInterface::Config &conf, ImVec2 size, int index);
	};
}