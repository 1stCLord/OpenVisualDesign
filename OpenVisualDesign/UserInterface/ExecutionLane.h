#pragma once
#include "Window.h"
#include "CallablesWindow.h"

namespace OVD
{
	class ExecutionLane : public Window
	{
	public:
		ExecutionLane(UserInterface* user_interface);
		void render() override;

		Definition * definition = nullptr;
	private:
		CallablesWindow callables_window;
		float current_size_y_scale = 0.8f;
		int insert_index = 0;
		int register_lanes = 1;

		void render_register_lanes_controls(bool render, float height);
		void render_register_lanes(bool render, float height);
	};
}