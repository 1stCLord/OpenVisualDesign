#include "ExecutionLane.h"

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
            callees.insert(callees.begin() + insert_index, std::make_unique<Definition::Callee>(selected));
        };
    }

	void ExecutionLane::render()
	{
        if (definition == nullptr)return;

        ImVec2 position = { (float)conf().calc_borders(1) + conf().left_win_size, conf().border };
        ImVec2 size = { conf().calc_fill(position).x, (float)conf().execution_lane_size + ((RegisterLane::register_lane_size+(conf().window_padding.y*2))*register_lanes.size()) + 10 };
        ImVec2 panel_size = { current_size_y_scale * conf().execution_lane_size, conf().execution_lane_size * 0.8f };
        float content_size_x = (panel_size.x + conf().window_padding.x * 2) * definition->get_callees().size();
        if (content_size_x < size.x)content_size_x = 0;

        ImGui::SetNextWindowPos(position);
        ImGui::SetNextWindowContentSize({content_size_x, 0});
        std::string name;
        if (definition != nullptr)
            name = definition->get_callable().name;

        ImGui::BeginChild(name.c_str(), size, true, ImGuiWindowFlags_HorizontalScrollbar);
        bool should_render_register_lanes = ImGui::CollapsingHeader("Register Lanes");

        render_register_lanes_controls(should_render_register_lanes);

        ImGui::BeginChild("lane");
        render_register_lanes(should_render_register_lanes);
        int i = 0;

        std::vector<std::unique_ptr<Definition::Callee>> &callees = definition->get_callees();

        bool cursor_in_window = Window::cursor_in_window(position, size);

        bool has_popup = callables_window.is_open();
        if(!has_popup)insert_index = 0;
        for (std::unique_ptr<Definition::Callee> &callee : callees)
        {
            if (panel_locations.size() < (i + 1))panel_locations.push_back(PanelLocations());

            
            CalleePanel::render_callee_panel(get_ui(), callee.get(), panel_size, i, has_popup, panel_locations[i]);
            if (ImGui::GetMousePos().x > panel_locations[i].panel_location.x && cursor_in_window && !has_popup)
                insert_index = i + 1;
            i++;
        }

        if(has_popup || cursor_in_window)
            render_drop_insert();
        RegisterLane::handle_static_drop(*get_ui());
        drop_callee(cursor_in_window ? insert_index : -1);

        ImGui::EndChild();
        ImGui::EndChild();
        
        if (ImGui::GetIO().KeyShift)ImGui::SetScrollX(ImGui::GetIO().MouseWheel / 10);
        else current_size_y_scale = std::max(0.1f, current_size_y_scale + ImGui::GetIO().MouseWheel/10);

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

    RegisterLane const * ExecutionLane::get_register_lane(Definition::Callee const* callee) const
    {
        for (const RegisterLane& lane : register_lanes)
        {
            if (lane.contains_callee(callee))return &lane;
        }
        return nullptr;
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
            int i = 0;
            for(RegisterLane &register_lane : register_lanes)
            {
                register_lane.render();
            }
            ImGui::EndChild();
        }
    }

    void ExecutionLane::render_drop_insert() const
    {
        if (!panel_locations.empty())
        {
            if (insert_index == 0)
            {
                ImVec2 drop_bar_location = { panel_locations[insert_index].before_panel_location.x - conf().window_padding.x, panel_locations[insert_index].before_panel_location.y + conf().border };
                ImVec2 drop_bar_end = { panel_locations[insert_index].before_panel_location.x, panel_locations[insert_index].end_panel_location.y - conf().border };
                ImGui::GetForegroundDrawList()->AddRectFilled(drop_bar_location, drop_bar_end, ImColor(0.9f, 0.1f, 0.1f, 0.2f), 0.5f);
            }
            else if (insert_index == panel_locations.size())
            {
                ImVec2 drop_bar_location = { panel_locations[insert_index - 1].end_panel_location.x - conf().window_padding.x, panel_locations[insert_index - 1].before_panel_location.y + conf().border };
                ImVec2 drop_bar_end = { panel_locations[insert_index-1].end_panel_location.x, panel_locations[insert_index-1].end_panel_location.y - conf().border };
                ImGui::GetForegroundDrawList()->AddRectFilled(drop_bar_location, drop_bar_end, ImColor(0.9f, 0.1f, 0.1f, 0.2f), 0.5f);
            }
            else if (panel_locations.size() > insert_index)
            {
                ImVec2 drop_bar_location = { panel_locations[insert_index-1].end_panel_location.x - conf().window_padding.x, panel_locations[insert_index-1].before_panel_location.y + conf().border };
                ImVec2 drop_bar_end = { panel_locations[insert_index].before_panel_location.x, panel_locations[insert_index].end_panel_location.y - conf().border };
                ImGui::GetForegroundDrawList()->AddRectFilled(drop_bar_location, drop_bar_end, ImColor(0.9f, 0.1f, 0.1f, 0.2f), 0.5f);
            }
        }
    }

    void ExecutionLane::drop_callee(int destination)
    {
        std::vector<std::unique_ptr<Definition::Callee>>& callees = definition->get_callees();

        if (ImGuiPayload const* payload = ImGui::GetDragDropPayload())
        {
            Definition::Callee const* payload_callee = *(Definition::Callee const**)payload->Data;
            RegisterLane* source_lane = *(((RegisterLane**)payload->Data) + 1);

            std::string payload_class = std::string(payload->DataType);
            if (payload_class == "callee_panel")
            {
                if (ImGui::AcceptDragDropPayload(payload_class.c_str()))
                {
                    accept_payload();
                    int source_index = *(int*)payload->Data;
                    Definition::Callee* callee = callees[source_index].get();
                    callees[source_index].release();

                    if (destination >= 0 && destination > source_index)
                        callees.insert(callees.begin() + destination, std::unique_ptr<Definition::Callee>(callee));
                    callees.erase(callees.begin() + source_index);
                    if (destination >= 0 && destination <= source_index)
                        callees.insert(callees.begin() + destination, std::unique_ptr<Definition::Callee>(callee));
                }
            }
        }
    }
}