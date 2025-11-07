cmake_minimum_required(VERSION 3.11)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# macOS specific settings
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
endif()

# Add executable with all source files
add_executable(snake 
    src/main.cpp
    src/game_state.cpp
    src/game_logic.cpp
    src/renderer.cpp
)

# Find raylib
find_package(raylib QUIET)

if(raylib_FOUND)
    message(STATUS "raylib found via CMake")
    target_link_libraries(snake raylib)
else()
    message(STATUS "raylib not found via CMake, trying pkg-config")
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(RAYLIB raylib)
        if(RAYLIB_FOUND)
            target_include_directories(snake PRIVATE ${RAYLIB_INCLUDE_DIRS})
            target_link_directories(snake PRIVATE ${RAYLIB_LIBRARY_DIRS})
            target_link_libraries(snake ${RAYLIB_LIBRARIES})
            target_compile_options(snake PRIVATE ${RAYLIB_CFLAGS_OTHER})
        else()
            message(FATAL_ERROR "raylib not found. Please install raylib.")
        endif()
    else()
        message(FATAL_ERROR "raylib not found and pkg-config not available. Please install raylib.")
    endif()
endif()

