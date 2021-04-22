#pragma once
#include "Window.h"
#include <set>
#include <map>

namespace OVD
{
	class ExecutionLane;

	class RegisterLane : public Window
	{
	public:
		RegisterLane(UserInterface* user_interface, int index, ExecutionLane const * parent);
		void render() override;
		static void handle_static_drop(UserInterface &ui);

		static float register_lane_size;
		static const int register_lane_max = 8;
		static const int register_lane_min = 1;
	private:

		using callees = std::set<Definition::Callee const*>;
		using ordered_callees = std::map<int, Definition::Callee const*>;

		int index;

		callees callee_results;

		ExecutionLane const * parent;

		ordered_callees get_ordered_callees() const;
		void render_drag(const std::string &name, Definition::Callee const* callee);
		void handle_drop();
	};
}