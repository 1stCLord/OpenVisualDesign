#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <imgui.h>
#include "OpenVisualDesign.h"

struct SDL_Window;
typedef void* SDL_GLContext;

namespace OVD
{
	class Window;
	class ExecutionLane;
	class DefinitionsWindow;

	class UserInterface
	{
	public:
		UserInterface();
		~UserInterface();

		void run();

		const std:: vector<ExecutionLane*>& get_execution_lanes() const;

		struct Config
		{
			ImVec2 window_size{ 1920, 1080 };
			ImVec2 window_padding = ImVec2(8, 4);
			ImVec2 main_view_size{ 1580, 720 };
			float border = 10;
			float left_win_size = 300;
			float bottom_win_size = 500;
			float execution_lane_size = 200;
			float half_line_height = 6.0f;

			ImColor window_colour = ImColor(0.45f, 0.55f, 0.60f, 1.00f);
			ImColor panel_colour = ImColor(0.16f, 0.16f, 0.16f, 1.00f);
			ImColor insert_colour = { .3f,.3f, .1f, 1.f };
			ImColor object_panel_colour = { .1f,.1f, .3f, 1.f };
			ImColor param_panel_colour = { .3f,.1f, .1f, 1.f };
			ImColor return_panel_colour = { .1f,.3f, .1f, 1.f };

			int calc_borders(int window_count);
			ImVec2 calc_fill(ImVec2 position);
		} config;
		OpenVisualDesign ovd;

		std::function<void(Definition *)> notify_selected_definition;

		bool payload_accepted = false;

	private:
		void update();
		bool poll();
		void render();

		void render_menu_bar();

		void add_execution_lane(Definition* selected);
		void remove_execution_lane(ExecutionLane* execution_lane);

		void update_selected_definitions();

		SDL_Window* window;
		SDL_GLContext gl_context;

		std::vector<std::unique_ptr<Window>> windows;
		DefinitionsWindow* definitions_window;
		std::vector<ExecutionLane*> execution_lanes;

		Definition* new_selected_definition = nullptr;
	};
}