#pragma once
#include "Window.h"

namespace OVD
{
	class CalleePanel : public Window
	{
	public:
		static bool render_callee_panel(Definition::Callee const * callee, ImVec2 size, int index, bool has_popup);
		static void render_panel_core(Definition::Callee const *callable, ImVec2 size, int index);
		static void render_dragdrop(Definition::Callee const * callable, ImVec2 size, int index);
		static void render_dragdrop_return(Definition::Callee const * value);
		static void render_grab_handle();
	};
}