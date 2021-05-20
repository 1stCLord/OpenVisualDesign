#include "DefinitionsWindow.h"
#include "Header.h"
#include "Definition.h"
#include <imgui.h>

namespace OVD
{
	void DefinitionsWindow::render()
	{
        ImVec2 position = { conf().border, conf().border };
        ImVec2 size(conf().left_win_size, conf().calc_fill(position).y);
        ImGui::SetNextWindowPos(position);
        char searchtext[50];
        memset(searchtext, 0, 50);
        ImGui::BeginChild("toolbar", size, true);
        ImGui::InputText("Search", searchtext, 50);
        for (std::unique_ptr<Header> &header : get_ui()->ovd.headers)
        {
            std::string filename = header->get_filename().filename().string();
            if (ImGui::TreeNode(filename.c_str()))
            {
                for (Scope& scope : header->get_scopes())
                {
                    for (Definition& definition : scope.get_definitions())
                    {
                        if (ImGui::Selectable(definition.get_callable().name.c_str(), (&definition) == selected))
                        {
                            selected = &definition;
                            (get_ui())->notify_selected_definition(selected);
                        }
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::EndChild();
	}
}
