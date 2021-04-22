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
		ImGui::BeginChild((std::string("registerlane") + std::to_string(index)).c_str(), { 0, RegisterLane::register_lane_size }, true, ImGuiWindowFlags_NoScrollbar);

		if (callee_results.size())
		{
			ordered_callees curent_ordered_callees = get_ordered_callees();
			
			int first_index = curent_ordered_callees.cbegin()->first;
			if (first_index != 0)
			{
				ImGui::BeginChild(std::string("spacer").c_str(), { first_index * parent->get_current_size_y_scale() * conf().execution_lane_size,0 });
				ImGui::EndChild();
				ImGui::SameLine();
			}

			for (ordered_callees::const_iterator reg_cit = curent_ordered_callees.cbegin(); reg_cit != curent_ordered_callees.cend(); ++reg_cit)
			{
				ordered_callees::const_iterator next_reg_cit = reg_cit;
				++next_reg_cit;
				int current_length = 0;
				if (next_reg_cit != curent_ordered_callees.cend())
				{
					current_length = next_reg_cit->first - reg_cit->first;
				}

				Definition::Callee const * callee = reg_cit->second;
				bool selected;

				if (parent->panel_locations.size() > reg_cit->first)
				{
					ImVec2 start = ImGui::GetCursorScreenPos(), end = parent->panel_locations[reg_cit->first].return_location;
					start.y += conf().half_line_height; end.y += conf().half_line_height;
					ImVec2 mid1 = { start.x - conf().half_line_height, start.y + conf().half_line_height }, mid2 = { start.x - conf().half_line_height, end.y - conf().half_line_height };
					ImGui::GetForegroundDrawList()->AddBezierCurve(start, mid1, mid2, end, ImColor{ .9f,.9f,.7f,1.f }, 2);
				}

				ImGui::Selectable((" " + callee->callable->return_type).c_str(), &selected, 0, { current_length * parent->get_current_size_y_scale() * conf().execution_lane_size,0 });
				render_drag(callee->callable->return_type, callee);
				ImGui::SameLine();
			}
		}

		ImGui::EndChild();

		handle_drop();

	}

	RegisterLane::ordered_callees RegisterLane::get_ordered_callees() const
	{
		std::map<int, Definition::Callee const*> result;
		for (Definition::Callee const* callee : callee_results)
		{
			int index = parent->get_column_of(callee);
			result[index] = callee;
		}
		return result;
	}

	void RegisterLane::render_drag(const std::string &name, Definition::Callee const* callee)
	{
		std::string register_name = std::string("register") + std::to_string(index);
		if (ImGui::BeginDragDropSource())
		{
			void const * pointers[2] = {callee, this};
			ImGui::SetDragDropPayload(register_name.c_str(), pointers, sizeof(callee) + sizeof(this));
			ImGui::Selectable(name.c_str());
			ImGui::EndDragDropSource();
		}
	}

	void RegisterLane::handle_drop()
	{
		std::string register_name = std::string("register") + std::to_string(index);
		ImGuiPayload const* payload = ImGui::GetDragDropPayload();
		if (ImGui::BeginDragDropTarget())
		{
			Definition::Callee const * payload_callee = *(Definition::Callee const **)payload->Data;
			RegisterLane * source_lane = *(((RegisterLane**)payload->Data) + 1);
			if (ImGui::AcceptDragDropPayload("callee_return"))
			{
				accept_payload();
				callee_results.insert(payload_callee);
			}

			std::string payload_class = std::string(payload->DataType);
			if (payload_class.starts_with("register") && payload_class != register_name)
			{
				if (ImGui::AcceptDragDropPayload(payload_class.c_str()))
				{
					accept_payload();
					callee_results.insert(payload_callee);
					source_lane->callee_results.erase(payload_callee);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	void  RegisterLane::handle_static_drop(UserInterface &ui)
	{
		ImGuiPayload const* payload = ImGui::GetDragDropPayload();
		if (payload && !ui.payload_accepted)
		{
			Definition::Callee const* payload_callee = *(Definition::Callee const**)payload->Data;
			RegisterLane* source_lane = *(((RegisterLane**)payload->Data) + 1);

			std::string payload_class = std::string(payload->DataType);
			if (payload_class.starts_with("register"))
			{
				if (ImGui::AcceptDragDropPayload(payload_class.c_str()))
				{
					ui.payload_accepted = true;
					source_lane->callee_results.erase(payload_callee);
				}
			}
		}
	}
}