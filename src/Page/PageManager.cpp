#include "Page/PageManager.h"

void PageManager::Register(std::shared_ptr<Page> page){
    PageManager::GetPages().push_back(std::move(page));
}

void PageManager::RenderAll(){
    for(auto& page : PageManager::GetPages()){
        page->renderPage();
    }
}

std::vector<std::shared_ptr<Page>>& PageManager::GetPages(){
    static std::vector<std::shared_ptr<Page>> pages;
    return pages;
}