# Noah's Mobile Development Toolkit (NMTK)

**NMTK** is a lightweight C++ toolkit for building simple, cross-platform utilities for **Linux** and **Android** using **SDL2** and **Dear ImGui**. It's ideal for developers looking to quickly spin up native tools with a modern, immediate-mode GUI.

------

## ✨ Features

- 🔄 **Cross-platform**: Build once, run on Linux and Android.
- 🧱 **Modular structure**: Plug in menus and pages with ease.
- 🎮 **Input management**: Centralized SDL event handling with callbacks.
- 🌿 **Immediate-mode GUI**: Powered by Dear ImGui.
- ✨ **Minimal boilerplate**: Focus on your logic, not your framework.

------

## 📦 Requirements

- C++17 or later
- [SDL2](https://www.libsdl.org/)
- [Dear ImGui](https://github.com/ocornut/imgui)

------

## 🧰 Architecture

### `InputManager`

Registers and dispatches SDL events to user-defined callbacks.

```cpp
InputManager::RegisterCallback([](const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        // Handle key press
    }
});
```

### `MenuManager`

Manages top-level menus and renders them using ImGui.

```cpp
auto fileMenu = std::make_shared<MenuGroup>();
fileMenu->Name = "File";
fileMenu->Items.push_back(std::make_shared<SimpleMenuItem>("Open", []() {
    // Action on click
}));
MenuManager::Register(fileMenu);
```

### `PageManager`

Registers and renders your application's pages.

```cpp
class MyPage : public Page {
    void Render() override {
        ImGui::Text("Hello from MyPage!");
    }
};

PageManager::Register(std::make_shared<MyPage>());
```

### `MenuItem` and `MenuGroup`

Define custom menu items and organize them into groups:

```cpp
class SimpleMenuItem : public MenuItem {
public:
    SimpleMenuItem(const std::string& label, std::function<void()> onClick)
        : Label(label), OnClick(onClick) {}

    void render() override {
        if (ImGui::MenuItem(Label.c_str())) {
            OnClick();
        }
    }

private:
    std::string Label;
    std::function<void()> OnClick;
};
```

------

## 🚀 Getting Started

```bash
git clone https://github.com/NoahGWood/NMTK.git
cd NMTK
# Build using your preferred system (e.g., CMake)
```

Here's a sample `main.cpp` to help you get started:

```cpp
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "App.h"
#include "Core/InputManager.h"
#include "Menu/MenuManager.h"
#include "Page/PageManager.h"

#ifdef __ANDROID__
#include <GLES3/gl3.h>
#else
#include <SDL_opengl.h>
#endif

// Include your pages & menus here:

#include "Page/Home.h"


void AppSetup(){
    // User-defined entry point
    // Feel free to add pages, menus, and other things here
    PageManager::Register(std::make_shared<HomePage>());
}

void ImGuiIOSetup(ImGuiIO& io){
    // User-defined ImGuiIO Setup
}

#ifdef __ANDROID__
extern "C" int SDL_main(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    // Setup
    AppSetup();
    // SDL + GL Init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Hello",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        480, 800, SDL_WINDOW_OPENGL);
    SDL_GLContext gl = SDL_GL_CreateContext(window);

    // ImGui Init
    ImGui::CreateContext();
    ImGuiIO& ImGui::GetIO();
    /// Set ImGui Flags
    ImGuiIOSetup(io);
    ImGui_ImplSDL2_InitForOpenGL(window, gl);
#ifdef __ANDROID
    ImGui_ImplOpenGL3_Init("#version 300 es");
#else
    ImGui_ImplOpenGL3_Init("#version 130");
#endif
    // Main loop-de-loop
    bool running = true;
    SDL_Event e;
    while(running){
        while(SDL_PollEvent(&e)){
            // Handle system events
            if(e.type == SDL_QUIT) running = false;
            // Pass events to ImGui
            ImGui_ImplSDL2_ProcessEvent(&e);
            // Pass events to InputManager
            InputManager::DispatchEvent(&e);
        }
        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        // Render UI Menus
        MenuManager::RenderAll();
        // Render UI Pages
        PageManager::RenderAll();
        // Finalize ImGui frame & render to screen
        ImGui::Render();
        // Make sure we're in proper context
        SDL_GL_MakeCurrent(window, gl);
        // Clear GL buffers (color/depth) before drawing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw data to the framebuffer
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Swap front/back framebuffers
        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
    }
    // Clean up OpenGL
    ImGui_ImplOpenGL3_Shutdown();
    // Clean up SDL2
    ImGui_ImplSDL2_shutdown();
    // Clean up ImGui
    ImGui::DestroyContext();
    // Clean up window
    SDL_DestroyWindow(window);
    // Quit application
    SDL_Quit();
    return 0;
}
```

------

## 📁 Directory Structure

```
include/
├── App.h
├── Core/
│   └── InputManager.h
├── Menu/
│   ├── MenuManager.h
│   ├── MenuItem.h
│   └── MenuGroup.h
├── Page/
    ├── Home.h
    ├── PageManager.h
    └── Page.h
src/
├── Core/
│   └── InputManager.cpp
├── Menu/
│   └── MenuManager.cpp
├── Page/
|   ├── Home.cpp
|   └── PageManager.cpp
└── main.cpp
vendor/
├── imgui/
├── SDL/
```

------

## 🔧 Roadmap

-  Navigation stack for multi-page apps
-  Built-in widgets/utilities (FPS counter, console log)
-  Android touch helpers
-  Example projects

------

## 👥 Contributing

Contributions are welcome! Open an issue or submit a PR with your improvements.

------

## 📄 License

[TBD]