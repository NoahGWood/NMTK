#pragma once
#include "Page/Page.h"
#include <vector>
#include <memory>

class PageManager
{
    public:
        static void Register(std::shared_ptr<Page> page);
        static void RenderAll();
    private:
        static std::vector<std::shared_ptr<Page>>& GetPages();
};