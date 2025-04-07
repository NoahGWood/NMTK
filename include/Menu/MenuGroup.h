#pragma once
#include "Menu/MenuItem.h"
#include <vector>
#include <string>
#include <memory>

struct MenuGroup
{
    std::string Name;
    std::vector<std::shared_ptr<MenuItem>> Items;
};
