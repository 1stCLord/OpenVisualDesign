#include "ExecutionLane.h"
#include "CalleePanel.h"

namespace OVD
{
    ExecutionLane::ExecutionLane(UserInterface* user_interface) :Window(user_interface), callables_window(user_interface), register_lanes{RegisterLane(user_interface,0, this)}
    {
        get_ui()->notify_selected_definition = [&](Definition * selected)
        {
            definition = selected;
        };

        callables_window.notify_selected_callable = [&](Callable const* selected)
        {
            std::vector<std::unique_ptr<Definition::Callee>> &callees = definition->get_callees();
            definition->get_callees().insert(callees.begin() + insert_index, std::make_unique<Definition::Callee>(selected));
        };
    }

	void ExecutionLane::render()
	{
        if (definition == nullptr)return;

        ImVec2 position = { (float)conf().calc_borders(1) + conf().left_win_size, conf().border };
        ImVec2 size = { conf().calc_fill(position).x, (float)conf().execution_lane_size + ((RegisterLane::register_lane_size+10)*register_lanes.size()) };
        ImGui::SetNextWindowPos(position);
        std::string name;
        if (definition != nullptr)
            name = definition->get_callable().name;

        ImGui::BeginChild(name.c_str(), size, true, ImGuiWindowFlags_HorizontalScrollbar);
        bool should_render_register_lanes = ImGui::CollapsingHeader("Register Lanes");

        render_register_lanes_controls(should_render_register_lanes);

        ImGui::BeginChild("lane");
        render_register_lanes(should_render_register_lanes);
        int i = 0, drop_source = -1;

        std::vector<std::unique_ptr<Definition::Callee>> &callees = definition->get_callees();

        size = { current_size_y_scale * conf().execution_lane_size, conf().execution_lane_size * 0.8f };
        int accept_drop = accept_callee_drop(size, -1);
        if (accept_drop >= 0)
        {
            insert_index = -1;
            drop_source = accept_drop;
        }

        for (std::unique_ptr<Definition::Callee> &callee : callees)
        {
            bool has_popup = callables_window.is_open() && insert_index == i;
            if (CalleePanel::render_callee_panel(callee.get(), size, i, has_popup))
                insert_index = i;
            int accept_drop = accept_callee_drop(size, i);
            if (accept_drop >= 0)
            {
                insert_index = i;
                drop_source = accept_drop;
            }
            i++;
        }

        drop_callee(drop_source, insert_index+1);

        ImGui::EndChild();
        ImGui::EndChild();
        current_size_y_scale = std::max(0.1f, current_size_y_scale + ImGui::GetIO().MouseWheel/10);

        callables_window.render();
	}

    int ExecutionLane::get_column_of(Definition::Callee const* callee) const
    {
        int i = 0;
        for (std::unique_ptr<Definition::Callee> &current_callee : definition->get_callees())
        {
            if (current_callee.get() == callee)
                return i;
            ++i;
        }
        return -1;
    }

    void ExecutionLane::render_register_lanes_controls(bool render)
    {
        ImGui::BeginChild("registerlanescontrols", { 30, RegisterLane::register_lane_size * register_lanes.size() * 1.3f }, false, ImGuiWindowFlags_NoScrollbar);
        if (render)
        {
            if (register_lanes.size() < RegisterLane::register_lane_max)
            {
                if (ImGui::Button("+", { RegisterLane::register_lane_size, RegisterLane::register_lane_size }))
                    register_lanes.push_back(RegisterLane(get_ui(), register_lanes.size(), this));
            }
            if (register_lanes.size() > RegisterLane::register_lane_min)
            {
                if (ImGui::Button("-", { RegisterLane::register_lane_size, RegisterLane::register_lane_size }))
                    register_lanes.pop_back();
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
    }

    void ExecutionLane::render_register_lanes(bool render)
    {
        if (render)
        {
            ImGui::BeginChild("registerlanes", { 0, RegisterLane::register_lane_size * register_lanes.size() * 1.3f }, false, ImGuiWindowFlags_HorizontalScrollbar);
            //for (int i = 0; i < register_lanes; ++i)
            int i = 0;
            for(RegisterLane &register_lane : register_lanes)
            {
                register_lane.render();
            }
            ImGui::EndChild();
            RegisterLane::handle_static_drop();
        }
    }

    void ExecutionLane::drop_callee(int source, int destination)
    {
        std::vector<std::unique_ptr<Definition::Callee>>& callees = definition->get_callees();

        if (source >= 0)
        {
            Definition::Callee *callee = callees[source].get();
            callees[source].release();
            if (destination > source)
                callees.insert(callees.begin() + destination, std::unique_ptr<Definition::Callee>(callee));
            callees.erase(callees.begin() + source);
            if (destination <= source)
                callees.insert(callees.begin() + destination, std::unique_ptr<Definition::Callee>(callee));
        }
    }

    int ExecutionLane::accept_callee_drop(ImVec2 size, int index)
    {
        ImGuiPayload const * payload = ImGui::GetDragDropPayload();
        if (payload && std::string(payload->DataType) == "callee_panel")
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, { .3f,.3f, .1f, 1.f });
            ImGui::BeginChild((std::string("insert") + std::to_string(index)).c_str(), { size.x * .1f, size.y });
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::PopStyleColor();
            if (ImGui::BeginDragDropTarget())
            {
                if (ImGuiPayload const * payload = ImGui::AcceptDragDropPayload("callee_panel"))
                {
                    int source_index = *(int*)payload->Data;
                    return source_index;
                }
                ImGui::EndDragDropTarget();
            }
        }
        return -1;
    }
}