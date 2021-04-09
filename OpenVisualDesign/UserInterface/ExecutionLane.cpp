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
            ImGui::GetMousePosOnOpeningCurrentPopup();
            Definition::Callee callee{ selected };
            std::vector<Definition::Callee> &callees = definition->get_callees();
            definition->get_callees().insert(callees.begin() + insert_index, callee);
            ImGui::CloseCurrentPopup();
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
        int i = 0;
        for (Definition::Callee& callee : definition->get_callees())
        {
            if (CalleePanel::render_callee_panel(callee, { current_size_y_scale * conf().execution_lane_size, conf().execution_lane_size * 0.8f }, i++))
                insert_index = i;
        }
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
}