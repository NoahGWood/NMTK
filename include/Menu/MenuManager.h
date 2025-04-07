#pragma once
#include "Menu/MenuItem.h"
#include "Menu/MenuGroup.h"
#include <vector>
#include <memory>

class MenuManager
{
    public:
        static void Register(std::shared_ptr<MenuGroup> menuGroup);
        static void RenderAll();
    private:
        static std::vector<std::shared_ptr<MenuGroup>>& GetMenuGroups();
};
