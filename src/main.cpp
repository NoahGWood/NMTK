#define SDL_MAIN_HANDLED
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
#include <android/log.h>
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

    // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#ifndef __ANDROID__
    // Enable multi-viewports in desktop builds
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
#endif
}

void UpdateTextInputState(){
    ImGuiIO& io = ImGui::GetIO();
    #ifdef __ANDROID__
    static bool keyboard_open = false;
    if(io.WantTextInput && !keyboard_open){
        SDL_StartTextInput();
        keyboard_open = true;
    } else if(!io.WantTextInput && keyboard_open){
        SDL_StopTextInput();
        keyboard_open = false;
    }
    #endif
}

#ifdef __ANDROID__
extern "C" int SDL_main(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    #ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "NMTK", "Hello from SDL_main()");
    #endif
    // Setup
    AppSetup();
    // SDL + GL Init
    SDL_Init(SDL_INIT_VIDEO);
#ifdef __ANDROID__
    SDL_Window* window = SDL_CreateWindow("NMTK",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI);
#else
    SDL_Window* window = SDL_CreateWindow("NMTK",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        480, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#endif
    SDL_GLContext gl = SDL_GL_CreateContext(window);
    // ImGui Init
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForOpenGL(window, gl);
#ifdef  __ANDROID__
    ImGui_ImplOpenGL3_Init("#version 300 es");
    // Handle scaling on android
    float dpi = 160.0f;
    float hdpi, wdpi;
    if(SDL_GetDisplayDPI(0, &dpi, &hdpi, &wdpi) == 0){
        io.FontGlobalScale = dpi / 160.0f; // 160 is baseline
    } else {
        io.FontGlobalScale = 2.0f; // Fallback default
    }
#else
    ImGui_ImplOpenGL3_Init("#version 130");
#endif
    /// Set ImGui Flags
    ImGuiIOSetup(io);
    // Main loop-de-loop
    bool running = true;
    SDL_Event e;

    #ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "NMTK", "Starting render loop!");
    #endif
    while(running){
        while(SDL_PollEvent(&e)){
            // Handle system events
            if(e.type == SDL_QUIT)
            {
                running = false;
            }
#ifndef __ANDROID__
            // Handle closing on multi-viewports
            else if (e.type == SDL_WINDOWEVENT){
                if(e.window.event == SDL_WINDOWEVENT_CLOSE){
                    if(e.window.windowID == SDL_GetWindowID(window)) {
                        running = false;
                    }
                }
            }
            // Handle resize
            if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED){
                int newW = e.window.data1;
                int newH = e.window.data2;
                glViewport(0, 0, newW, newH);
            }
#endif
            // Pass events to ImGui
            ImGui_ImplSDL2_ProcessEvent(&e);
            // Pass events to InputManager
            InputManager::DispatchEvent(e);
        }
        // Let ImGui tell us when it needs text input
        UpdateTextInputState();
        int fb_width, fb_height;
        SDL_GL_GetDrawableSize(window, &fb_width, &fb_height);
        io.DisplaySize = ImVec2((float)fb_width, (float)fb_height);
        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        // Render dockspace if enabled
        if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
            ImGui::DockSpaceOverViewport();
        }
        // Render UI Menus
        MenuManager::RenderAll();
        // Render UI Pages
        PageManager::RenderAll();
        // Why not render demo window?
        ImGui::ShowDemoWindow();
        // Finalize ImGui frame & render to screen
        ImGui::Render();
        // Update platform windows if multi-viewport
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
        // Make sure we're in proper context
        // SDL_GL_MakeCurrent(window, gl);
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
    ImGui_ImplSDL2_Shutdown();
    // Clean up ImGui
    ImGui::DestroyContext();
    // Clean up window
    SDL_DestroyWindow(window);
    // Quit application
    SDL_Quit();
    return 0;
}