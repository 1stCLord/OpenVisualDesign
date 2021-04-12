#include "RegisterLane.h"
#include "ExecutionLane.h"

namespace OVD
{
	float RegisterLane::register_lane_size = 20.0f;

	RegisterLane::RegisterLane(UserInterface* user_interface, int index, ExecutionLane const * parent) : Window(user_interface), index(index), parent(parent)
	{

	}

	void RegisterLane::render()
	{
		ImGui::BeginChild((std::string("registerlane") + std::to_string(index)).c_str(), { 0, RegisterLane::register_lane_size }, true, ImGuiWindowFlags_HorizontalScrollbar);
		int column = 0;
		for (Definition::Callee const* callee : callee_results)
		{
			//todo need a map of indexes to callees
			int index = parent->get_column_of(callee);

			if (index >= 0)
			{

				while (column++ < index)
				{
					ImGui::BeginChild((std::string("column") + std::to_string(column)).c_str(), { parent->get_current_size_y_scale() * conf().execution_lane_size,0 });
					ImGui::EndChild();
					ImGui::SameLine();
				}

				bool selected;
				ImGui::Selectable(callee->callable->return_type.c_str(), &selected, 0, { parent->get_current_size_y_scale() * conf().execution_lane_size,0 });
				ImGui::SameLine();
			}
		}
		ImGui::EndChild();

		if (ImGui::BeginDragDropTarget())
		{
			if (ImGuiPayload const *payload = ImGui::AcceptDragDropPayload("callee_return"))
			{
				callee_results.insert(*(Definition::Callee**)payload->Data);
			}
			ImGui::EndDragDropTarget();
		}

	}
}