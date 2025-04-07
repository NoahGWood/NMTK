#pragma once
#include "Page/Page.h"

class HomePage : public Page
{
    public:
        HomePage() : Page("Home Page") {}
        void render() override;
};
