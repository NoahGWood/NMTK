cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=default")

if(ANDROID)
    set(MOBILE ON)
else()
    set(MOBILE ON)
endif()

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/SDL)

# ImGui
file(GLOB IMGUI_SOURCES
    vendor/imgui/*.cpp
    vendor/imgui/backends/imgui_impl_sdl2.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    vendor/imgui
    vendor/imgui/backends
    vendor/SDL/include
)

# MyApp
file(GLOB_RECURSE SOURCES src/*cpp)

if(ANDROID)
    add_library(main SHARED ${SOURCES}) # Android expects shared add_library
    target_include_directories(main PRIVATE
        include
        include/Core
        include/Menu
        include/Page
        vendor/SDL/include
        vendor/imgui
        vendor/imgui/backends
    )

else()
    add_executable(myapp ${SOURCES})
    target_include_directories(myapp PRIVATE
        include
        include/Core
        include/Menu
        include/Page
        vendor/SDL/include
        vendor/imgui
        vendor/imgui/backends
    )
endif()

if(ANDROID)
    target_link_libraries(main
        imgui
        SDL2-static
        log # Android logging
        android # Native activity, JNI, etc.
        EGL # OpenGL ES Context
        GLESv3 # OpenGL ES 3
        atomic # Needed for std::atmoic
        m # Math library (sin, cos, sqrt, etc)
    )
else()
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    target_link_libraries(myapp SDL2 imgui OpenGL::GL)
endif()