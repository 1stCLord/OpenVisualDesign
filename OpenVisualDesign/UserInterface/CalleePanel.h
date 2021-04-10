#pragma once
#include "Window.h"

namespace OVD
{
	class CalleePanel : public Window
	{
	public:
		static bool render_callee_panel(const Definition::Callee& callee, ImVec2 size, int index, bool has_popup);
		static void render_panel_core(const Callable *callable, ImVec2 size, int index);
		static void render_dragdrop(const Callable* callable, ImVec2 size, int index);
		static void render_dragdrop(const std::string &value);
		static void render_grab_handle();
	};
}