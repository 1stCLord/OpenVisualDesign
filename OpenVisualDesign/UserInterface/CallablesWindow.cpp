#include "CallablesWindow.h"
#include <imgui.h>
#include <algorithm>

namespace OVD
{
	void CallablesWindow::render()
	{
        ImGui::OpenPopupOnItemClick("callables_popup");
        if (ImGui::BeginPopup("callables_popup"))
        {
            open = true;
            ImVec2 size = { 200, 400 };
            char searchtext[50];
            memset(searchtext, 0, 50);
            ImGui::BeginChild("callables", size, true);
            ImGui::InputText("Search", searchtext, 50);
            ImGui::Text("todo");
            for (std::unique_ptr<Header>& header : get_ui()->ovd.headers)
            {
                std::string filename = header->get_filename().filename().string();
                for (const Callable& callable : header->get_callables())
                {
                    if (ImGui::Selectable(callable.name.c_str(), (&callable) == selected))
                    {
                        selected = &callable;
                        notify_selected_callable(selected);
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            ImGui::EndChild();

            ImGui::EndPopup();
        }
        else open = false;
	}
}
