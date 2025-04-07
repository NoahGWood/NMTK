#pragma once
#include <SDL.h>
#include <functional>
#include <vector>

class InputManager {
    public:
        using Callback = std::function<void(const SDL_Event&)>;

        static void RegisterCallback(Callback cb);
        static void DispatchEvent(const SDL_Event& e);

    private:
        static std::vector<Callback>& GetCallbacks();
};