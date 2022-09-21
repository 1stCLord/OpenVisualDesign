#include "CalleePanel.h"
#include "RegisterLane.h"
#include "ExecutionLane.h"

namespace OVD
{
    void CalleePanel::render()
    {
        locations.before_panel_location = ImGui::GetCursorScreenPos();
        ImGui::BeginChild((callee->callable->name + std::to_string(index)).c_str(), size, true);
        locations.panel_location = ImGui::GetCursorScreenPos();
        render_dragdrop();
        render_panel_core(false);
        ImGui::EndChild();
        locations.end_panel_location.y = ImGui::GetCursorScreenPos().y;
        ImGui::SameLine();
        locations.end_panel_location.x = ImGui::GetCursorScreenPos().x;
        render_param_inputs();
    }

    void CalleePanel::render_dragdrop()
    {
        Callable  const* callable = callee->callable;
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("callee_panel", &index, sizeof(index));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf().panel_colour.Value);
            ImGui::BeginChild((callable->name + std::to_string(index) + "dragdrop").c_str(), size, true);
            render_panel_core(true);
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::EndDragDropSource();
        }
    }

    void CalleePanel::render_grab_handle()
    {
        ImVec2 pos = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + conf().half_line_height };
        ImGui::GetWindowDrawList()->AddCircleFilled(pos, 4, ImColor{ .1f,.3f,.1f,1.f });
        ImGui::GetWindowDrawList()->AddCircle(pos, 4, ImColor{ .1f,.1f,.3f,1.f });
        ImGui::Spacing(); ImGui::SameLine();
    }

    void CalleePanel::render_dragdrop_return()
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("callee_return", &callee, sizeof(callee));
            ImGui::Selectable(callee->callable->return_type.c_str());
            ImGui::EndDragDropSource();
        }
    }

    void OVD::CalleePanel::render_param_inputs()
    {
        for (Definition::Callee const* parameter_source : callee->parameter_sources)
        {
            RegisterLane const* lane = parent->get_register_lane(parameter_source);
            if (lane)
            {
                ImVec2 start = lane->lane_location, end = locations.param_location;
                start.x = end.x;
                start.y += conf().half_line_height; end.y += conf().half_line_height;
                ImVec2 mid1 = { start.x - conf().half_line_height, start.y + conf().half_line_height }, mid2 = { start.x - conf().half_line_height, end.y - conf().half_line_height };
                ImGui::GetForegroundDrawList()->AddBezierCurve(start, mid1, mid2, end, ImColor{ .9f,.9f,.7f,1.f }, 2);
            }
        }
    }

    void CalleePanel::render_panel_core(bool is_dragdrop)
    {
        Callable const* callable = callee->callable;
        ImGui::Selectable(callable->name.c_str());

        if (!callable->classname.empty())
        {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf().object_panel_colour.Value);
            ImGui::BeginChild((callable->name + std::to_string(index) + "object").c_str(), { 0, 0.15f * size.y }, true);
            render_dragdrop();
            render_grab_handle();
            ImGui::Selectable(callable->classname.c_str());
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, conf().param_panel_colour.Value);
        ImGui::BeginChild((callable->name + std::to_string(index) + "params").c_str(), { 0, 0.4f * size.y }, true);
        if(!is_dragdrop)locations.param_location = ImGui::GetCursorScreenPos();
        render_dragdrop();
        uint8_t param_index = 0;
        for (const Variable& parameter : callable->parameters)
        {
            render_grab_handle();
            ImGui::Selectable(parameter.get_name().c_str());
            if(!is_dragdrop)
               drop_param(param_index);
            ++param_index;
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        if (callable->return_type != "void")
        {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf().return_panel_colour.Value);
            ImGui::BeginChild((callable->name + std::to_string(index) + "return").c_str(), { 0, 0.15f * size.y }, true);
            if (!is_dragdrop)locations.return_location = ImGui::GetCursorScreenPos();
            render_dragdrop();
            render_grab_handle();
            ImGui::Selectable(callable->return_type.c_str());
            render_dragdrop_return();
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
    }

    void CalleePanel::drop_param(uint8_t index)
    {
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::GetDragDropPayload();
            if (payload)
            {
                Definition::Callee const* payload_callee = *(Definition::Callee const**)payload->Data;
                RegisterLane* source_lane = *(((RegisterLane**)payload->Data) + 1);

                std::string payload_class = std::string(payload->DataType);
                if (payload_class.starts_with("register"))
                {
                    if (ImGui::AcceptDragDropPayload(payload_class.c_str()))
                    {
                        get_ui()->payload_accepted = true;
                        //todo type check, position check, draw line
                        callee->parameter_sources[index] = payload_callee;
                        
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

    }
}