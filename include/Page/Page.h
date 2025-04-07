#pragma once
#include <string>
#include <imgui.h>

class Page
{
    public:
        explicit Page(const std::string& name) : m_Name(name) {}
        virtual ~Page() = default;
        void renderPage() {
            if(ImGui::Begin(m_Name.c_str())){
                try{
                    render();
                } catch(...) {}
                ImGui::End(); // Always call end if begin() returns true;
            } else {
                ImGui::End(); // Must call end if Begin() returns false
            }
        }
        virtual void render() = 0;
    private:
        std::string m_Name;
};