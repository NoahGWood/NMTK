#include "Core/InputManager.h"

void InputManager::RegisterCallback(InputManager::Callback cb){
    InputManager::GetCallbacks().push_back(std::move(cb));
}

void InputManager::DispatchEvent(const SDL_Event& e){
    for(const auto& cb : InputManager::GetCallbacks()){
        cb(e);
    }
}

std::vector<InputManager::Callback>& InputManager::GetCallbacks() {
    static std::vector<InputManager::Callback> callbacks;
    return callbacks;
}