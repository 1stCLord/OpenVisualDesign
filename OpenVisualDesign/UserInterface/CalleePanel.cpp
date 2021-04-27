#include "CalleePanel.h"
#include "RegisterLane.h"

namespace OVD
{
    bool CalleePanel::render_callee_panel(UserInterface *ui, Definition::Callee * callee, ImVec2 size, int index, bool has_popup, PanelLocations &locations)
    {
        const UserInterface::Config& conf = ui->config;

        locations.before_panel_location = ImGui::GetCursorScreenPos();
        ImGui::BeginChild((callee->callable->name + std::to_string(index)).c_str(), size, true);
        locations.panel_location = ImGui::GetCursorScreenPos();
        render_dragdrop(conf, callee, size, index);
        render_panel_core(conf, callee, size, index, &locations, ui);
        ImGui::EndChild();
        locations.end_panel_location.y = ImGui::GetCursorScreenPos().y;
        ImGui::SameLine();
        locations.end_panel_location.x = ImGui::GetCursorScreenPos().x;
        //todo
        //render_param_inputs( callee, locations.param_location);

        /*
        bool hovered = ImGui::IsItemHovered();
        if (hovered || has_popup)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf.insert_colour.Value);
            ImGui::BeginChild("insert", { size.x * .1f, size.y });
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::PopStyleColor();
            return hovered;
        }*/
        return false;
    }

    void CalleePanel::render_dragdrop(const UserInterface::Config &conf, Definition::Callee * callee, ImVec2 size, int index)
    {
        Callable  const* callable = callee->callable;
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("callee_panel", &index, sizeof(index));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf.panel_colour.Value);
            ImGui::BeginChild((callable->name + std::to_string(index) + "dragdrop").c_str(), size, true);
            render_panel_core(conf, callee, size, index, nullptr);
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::EndDragDropSource();
        }
    }

    void CalleePanel::render_grab_handle(const UserInterface::Config &conf)
    {
        ImVec2 pos = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + conf.half_line_height };
        ImGui::GetWindowDrawList()->AddCircleFilled(pos, 4, ImColor{ .1f,.3f,.1f,1.f });
        ImGui::GetWindowDrawList()->AddCircle(pos, 4, ImColor{ .1f,.1f,.3f,1.f });
        ImGui::Spacing(); ImGui::SameLine();
    }

    void CalleePanel::render_dragdrop_return(Definition::Callee const *value)
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("callee_return", &value, sizeof(value));
            ImGui::Selectable(value->callable->return_type.c_str());
            ImGui::EndDragDropSource();
        }
    }

    void OVD::CalleePanel::render_param_inputs(const ImVec2 &register_lane_location, const ImVec2 &param_location )
    {
        ImVec2 start = register_lane_location, end = param_location;
        //start.y += conf().half_line_height; end.y += conf().half_line_height;
        //ImVec2 mid1 = { start.x - conf().half_line_height, start.y + conf().half_line_height }, mid2 = { start.x - conf().half_line_height, end.y - conf().half_line_height };
        ///ImGui::GetForegroundDrawList()->AddBezierCurve(start, mid1, mid2, end, ImColor{ .9f,.9f,.7f,1.f }, 2);
        ImGui::GetForegroundDrawList()->AddLine(start, end, ImColor{ .9f,.9f,.7f,1.f }, 2);
    }

    void CalleePanel::render_panel_core(const UserInterface::Config &conf, Definition::Callee *callee, ImVec2 size, int index, PanelLocations *locations, UserInterface *ui)
    {
        Callable const* callable = callee->callable;
        ImGui::Selectable(callable->name.c_str());

        if (!callable->classname.empty())
        {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf.object_panel_colour.Value);
            ImGui::BeginChild((callable->name + std::to_string(index) + "object").c_str(), { 0, 0.15f * size.y }, true);
            render_dragdrop(conf, callee, size, index);
            render_grab_handle(conf);
            ImGui::Selectable(callable->classname.c_str());
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, conf.param_panel_colour.Value);
        ImGui::BeginChild((callable->name + std::to_string(index) + "params").c_str(), { 0, 0.4f * size.y }, true);
        if (locations)locations->param_location = ImGui::GetCursorScreenPos();
        render_dragdrop(conf, callee, size, index);
        uint8_t param_index = 0;
        for (const Variable& parameter : callable->parameters)
        {
            render_grab_handle(conf);
            ImGui::Selectable(parameter.name.c_str());
            if(ui!=nullptr)
                drop_param(ui, callee, param_index);
            ++param_index;
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        if (callable->return_type != "void")
        {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf.return_panel_colour.Value);
            ImGui::BeginChild((callable->name + std::to_string(index) + "return").c_str(), { 0, 0.15f * size.y }, true);
            if (locations)locations->return_location = ImGui::GetCursorScreenPos();
            render_dragdrop(conf, callee, size, index);
            render_grab_handle(conf);
            ImGui::Selectable(callable->return_type.c_str());
            render_dragdrop_return(callee);
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
    }

    void CalleePanel::drop_param(UserInterface *ui, Definition::Callee * callee, uint8_t index)
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
                        ui->payload_accepted = true;
                        //todo type check, position check, draw line
                        callee->parameter_sources[index] = payload_callee;
                        
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

    }
}