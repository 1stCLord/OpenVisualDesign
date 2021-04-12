#pragma once
#include "Window.h"
#include <set>

namespace OVD
{
	class ExecutionLane;

	class RegisterLane : public Window
	{
	public:
		RegisterLane(UserInterface* user_interface, int index, ExecutionLane const * parent);
		void render() override;
		static float register_lane_size;
		static const int register_lane_max = 8;
		static const int register_lane_min = 1;
	private:
		int index;

		std::set<Definition::Callee const *> callee_results;

		ExecutionLane const * parent;
	};
}