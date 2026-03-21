cmake_minimum_required(VERSION 3.10)
project(Vigasoco)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Output structure
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/VigasocoSDL)
set(PLUGIN_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins)

# Global include paths
include_directories("${CMAKE_SOURCE_DIR}/core")
include_directories("${CMAKE_SOURCE_DIR}/core/abadia")
include_directories("${CMAKE_SOURCE_DIR}/core/util")
include_directories("${CMAKE_SOURCE_DIR}/core/systems")
include_directories("${CMAKE_SOURCE_DIR}/VigasocoSDL")

# Collect all core sources
file(GLOB_RECURSE CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/core/*.cpp"
)

# Exclude template implementation files that are #included directly
list(REMOVE_ITEM CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/core/Operations.cpp"
    "${CMAKE_SOURCE_DIR}/core/util/NotificationProvider.cpp"
)

# Build core as a static library
add_library(vigasoco_core STATIC ${CORE_SOURCES})
target_include_directories(vigasoco_core PUBLIC
    "${CMAKE_SOURCE_DIR}/core"
    "${CMAKE_SOURCE_DIR}/core/abadia"
    "${CMAKE_SOURCE_DIR}/core/util"
    "${CMAKE_SOURCE_DIR}/core/systems"
)

# Subdirectories
add_subdirectory(SDLInputKeyboardPlugin)
add_subdirectory(SDLVideoPlugins)
add_subdirectory(SDLAudioPlugin)
add_subdirectory(NULLAudioPlugin)
add_subdirectory(VigasocoSDL)