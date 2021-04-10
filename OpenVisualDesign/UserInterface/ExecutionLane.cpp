#include "ExecutionLane.h"
#include "CalleePanel.h"

namespace OVD
{
    ExecutionLane::ExecutionLane(UserInterface* user_interface) :Window(user_interface), callables_window(user_interface)
    {
        get_ui()->notify_selected_definition = [&](Definition * selected)
        {
            definition = selected;
        };

        callables_window.notify_selected_callable = [&](Callable const* selected)
        {
            Definition::Callee callee{ selected };
            std::vector<Definition::Callee> &callees = definition->get_callees();
            definition->get_callees().insert(callees.begin() + insert_index, callee);
        };
    }

	void ExecutionLane::render()
	{
        if (definition == nullptr)return;

        ImVec2 position = { (float)conf().calc_borders(1) + conf().left_win_size, conf().border };
        ImVec2 size = { conf().calc_fill(position).x, (float)conf().execution_lane_size + (30*register_lanes) };
        ImGui::SetNextWindowPos(position);
        std::string name;
        if (definition != nullptr)
            name = definition->get_callable().name;

        ImGui::BeginChild(name.c_str(), size, true, ImGuiWindowFlags_HorizontalScrollbar);
        bool should_render_register_lanes = ImGui::CollapsingHeader("Register Lanes");

        render_register_lanes_controls(should_render_register_lanes, 20);

        ImGui::BeginChild("lane");
        render_register_lanes(should_render_register_lanes, 20);
        int i = 0, drop_source = -1;

        std::vector<Definition::Callee> &callees = definition->get_callees();

        size = { current_size_y_scale * conf().execution_lane_size, conf().execution_lane_size * 0.8f };
        int accept_drop = accept_callee_drop(size, -1);
        if (accept_drop >= 0)
        {
            insert_index = -1;
            drop_source = accept_drop;
        }

        for (Definition::Callee& callee : callees)
        {
            bool has_popup = callables_window.is_open() && insert_index == i;
            if (CalleePanel::render_callee_panel(callee, size, i, has_popup))
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

    void ExecutionLane::render_register_lanes_controls(bool render, float height)
    {
        ImGui::BeginChild("registerlanescontrols", { 30, height * register_lanes * 1.3f }, false, ImGuiWindowFlags_NoScrollbar);
        if (render)
        {
            if (ImGui::Button("+", { height, height }))
                ++register_lanes;
            if (register_lanes > 1)
            {
                if (ImGui::Button("-", { height, height }))
                    --register_lanes;
            }
            register_lanes = std::max(1, register_lanes);
        }
        ImGui::EndChild();
        ImGui::SameLine();
    }

    void ExecutionLane::render_register_lanes(bool render, float height)
    {
        if (render)
        {
            ImGui::BeginChild("registerlanes", { 0, height * register_lanes * 1.3f }, false, ImGuiWindowFlags_HorizontalScrollbar);
            for (int i = 0; i < register_lanes; ++i)
            {
                ImGui::BeginChild((std::string("registerlane") + std::to_string(i)).c_str(), { 0, height }, true, ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
    }

    void ExecutionLane::drop_callee(int source, int destination)
    {
        std::vector<Definition::Callee>& callees = definition->get_callees();

        if (source >= 0)
        {
            Definition::Callee callee = callees[source];
            if (destination > source)
                callees.insert(callees.begin() + destination, callee);
            callees.erase(callees.begin() + source);
            if (destination <= source)
                callees.insert(callees.begin() + destination, callee);
        }
    }

    int ExecutionLane::accept_callee_drop(ImVec2 size, int index)
    {
        if (ImGui::GetDragDropPayload())
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, { .3f,.3f, .1f, 1.f });
            ImGui::BeginChild((std::string("insert") + std::to_string(index)).c_str(), { size.x * .1f, size.y });
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::PopStyleColor();
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("callee_panel"))
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