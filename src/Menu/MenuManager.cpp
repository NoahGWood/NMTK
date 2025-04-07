#include "Menu/MenuManager.h"
#include <imgui.h>

void MenuManager::Register(std::shared_ptr<MenuGroup> menuGroup){
    MenuManager::GetMenuGroups().push_back(std::move(menuGroup));
}

void MenuManager::RenderAll(){
    for(auto& mg : MenuManager::GetMenuGroups()){
        if(ImGui::BeginMenu(mg->Name.c_str())){
            for(auto& item : mg->Items){
                item->render();
            }
            ImGui::EndMenu();
        }
    }
}

std::vector<std::shared_ptr<MenuGroup>>& MenuManager::GetMenuGroups(){
    static std::vector<std::shared_ptr<MenuGroup>> menuGroups;
    return menuGroups;
}