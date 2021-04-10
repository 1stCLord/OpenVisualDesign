#include "CalleePanel.h"

namespace OVD
{
    bool CalleePanel::render_callee_panel(const Definition::Callee& callee, ImVec2 size, int index, bool has_popup)
    {
        ImGui::BeginChild((callee.callable->name + std::to_string(index)).c_str(), size, true);
        render_dragdrop(callee.callable, size, index);
        render_panel_core(callee.callable, size, index);
        ImGui::EndChild();
        ImGui::SameLine();

        bool hovered = ImGui::IsItemHovered();
        if (hovered || has_popup)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, { .3f,.3f, .1f, 1.f });
            ImGui::BeginChild("insert", { size.x * .1f, size.y });
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::PopStyleColor();
            return hovered;
        }
        return false;
    }

    void CalleePanel::render_dragdrop(const Callable* callable, ImVec2 size, int index)
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("callee_panel", &index, sizeof(index));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1.00f));
            ImGui::BeginChild((callable->name + std::to_string(index) + "dragdrop").c_str(), size, true);
            render_panel_core(callable, size, index);
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::EndDragDropSource();
        }
    }

    void CalleePanel::render_grab_handle()
    {
        ImVec2 pos = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 6 };
        ImGui::GetWindowDrawList()->AddCircleFilled(pos, 4, ImColor{ .1f,.3f,.1f,1.f });
        ImGui::GetWindowDrawList()->AddCircle(pos, 4, ImColor{ .1f,.1f,.3f,1.f });
        ImGui::Spacing(); ImGui::SameLine();
    }

    void CalleePanel::render_dragdrop(const std::string& value)
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::Selectable(value.c_str());
            ImGui::EndDragDropSource();
        }
    }

    void CalleePanel::render_panel_core(const Callable *callable, ImVec2 size, int index)
    {
        ImGui::Selectable(callable->name.c_str());

        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, { .3f,.1f, .1f, 1.f });
        ImGui::BeginChild((callable->name + std::to_string(index) + "params").c_str(), { 0, 0.5f * size.y }, true);
        render_dragdrop(callable, size, index);
        for (const Variable& parameter : callable->parameters)
        {
            render_grab_handle();
            ImGui::Selectable(parameter.name.c_str());
            render_dragdrop(parameter.name);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, { .1,.3, .1, 1 });
        ImGui::BeginChild((callable->name + std::to_string(index) + "return").c_str(), { 0, 0.15f * size.y }, true);
        render_dragdrop(callable, size, index);
        render_grab_handle();
        ImGui::Selectable(callable->return_type.c_str());
        render_dragdrop(callable->return_type);
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
}