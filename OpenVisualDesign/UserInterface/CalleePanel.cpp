#include "CalleePanel.h"

namespace OVD
{
    bool CalleePanel::render_callee_panel(const UserInterface::Config &conf, Definition::Callee const * callee, ImVec2 size, int index, bool has_popup, PanelLocations &locations)
    {
        ImGui::BeginChild((callee->callable->name + std::to_string(index)).c_str(), size, true);
        locations.panel_location = ImGui::GetCursorScreenPos();
        render_dragdrop(conf, callee, size, index);
        render_panel_core(conf, callee, size, index, &locations);
        ImGui::EndChild();
        ImGui::SameLine();

        bool hovered = ImGui::IsItemHovered();
        if (hovered || has_popup)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, conf.insert_colour.Value);
            ImGui::BeginChild("insert", { size.x * .1f, size.y });
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::PopStyleColor();
            return hovered;
        }
        return false;
    }

    void CalleePanel::render_dragdrop(const UserInterface::Config &conf, Definition::Callee const * callee, ImVec2 size, int index)
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

    void CalleePanel::render_panel_core(const UserInterface::Config &conf, Definition::Callee const *callee, ImVec2 size, int index, PanelLocations *locations)
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
        for (const Variable& parameter : callable->parameters)
        {
            render_grab_handle(conf);
            ImGui::Selectable(parameter.name.c_str());
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
}