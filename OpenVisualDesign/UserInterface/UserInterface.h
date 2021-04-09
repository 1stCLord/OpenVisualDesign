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

	class UserInterface
	{
	public:
		UserInterface();
		~UserInterface();

		void run();

		struct Config
		{
			ImVec2 window_size{ 1920, 1080 };
			ImVec2 main_view_size{ 1580, 720 };
			float border = 10;
			float left_win_size = 300;
			float bottom_win_size = 500;
			float execution_lane_size = 200;

			int calc_borders(int window_count);
			ImVec2 calc_fill(ImVec2 position);
		} config;
		OpenVisualDesign ovd;

		std::function<void(Definition *)> notify_selected_definition;
	private:
		void update();
		bool poll();
		void render();

		SDL_Window* window;
		SDL_GLContext gl_context;

		std::vector<std::unique_ptr<Window>> windows;
	};
}