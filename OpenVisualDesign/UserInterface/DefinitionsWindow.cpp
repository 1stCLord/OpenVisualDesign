#include "DefinitionsWindow.h"
#include "Header.h"
#include "Definition.h"
#include <imgui.h>

namespace OVD
{
	void DefinitionsWindow::render()
	{
        ImVec2 size(conf().left_win_size, 0);
        char searchtext[50];
        memset(searchtext, 0, 50);
        ImGui::BeginChild("toolbar", size, true, ImGuiWindowFlags_NoScrollbar);
        ImGui::InputText("Search", searchtext, 50);
        for (std::unique_ptr<Header> &header : get_ui()->ovd.headers)
        {
            std::string filename = header->get_filename().filename().string();
            if (ImGui::TreeNode(filename.c_str()))
            {
                for (Definition& definition : header->get_definitions())
                {
                    if (ImGui::Selectable(definition.get_callable().name.c_str(), (&definition) == selected))
                    {
                        selected = &definition;
                        (get_ui())->notify_selected_definition(selected);
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::EndChild();
	}
}
