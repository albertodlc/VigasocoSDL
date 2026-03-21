cmake_minimum_required(VERSION 3.10)
project(Vigasoco)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

# Global include paths
include_directories("${CMAKE_SOURCE_DIR}/core")
include_directories("${CMAKE_SOURCE_DIR}/core/abadia")
include_directories("${CMAKE_SOURCE_DIR}/core/util")
include_directories("${CMAKE_SOURCE_DIR}/core/systems")
include_directories("${CMAKE_SOURCE_DIR}/platform/sdl2")
include_directories("${CMAKE_SOURCE_DIR}/platform/sdl2/video")
include_directories("${CMAKE_SOURCE_DIR}/platform/sdl2/audio")
include_directories("${CMAKE_SOURCE_DIR}/platform/sdl2/input")

# Collect core sources
file(GLOB_RECURSE CORE_SOURCES "${CMAKE_SOURCE_DIR}/core/*.cpp")

# Exclude template implementation files included directly by headers
list(REMOVE_ITEM CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/core/Operations.cpp"
    "${CMAKE_SOURCE_DIR}/core/util/NotificationProvider.cpp"
)

# Build core as a static library
add_library(vigasoco_core STATIC ${CORE_SOURCES})
target_include_directories(vigasoco_core PUBLIC
    "${CMAKE_SOURCE_DIR}/core"
    "${CMAKE_SOURCE_DIR}/core/abadia"
    "${CMAKE_SOURCE_DIR}/core/abadia/ui"
    "${CMAKE_SOURCE_DIR}/core/util"
    "${CMAKE_SOURCE_DIR}/core/systems"
)

# Collect platform sources
file(GLOB_RECURSE PLATFORM_SOURCES "${CMAKE_SOURCE_DIR}/platform/sdl2/*.cpp")

# Exclude template implementation files
list(REMOVE_ITEM PLATFORM_SOURCES
    "${CMAKE_SOURCE_DIR}/platform/sdl2/video/SDLBasicDrawPluginTemplates.cpp"
)

# Build executable
add_executable(vigasoco ${PLATFORM_SOURCES})

# Link everything
target_link_libraries(vigasoco PRIVATE
    vigasoco_core
    ${SDL2_LIBRARIES}
    SDL2_ttf::SDL2_ttf
    dl
)

# Include paths for executable
target_include_directories(vigasoco PRIVATE
    "${CMAKE_SOURCE_DIR}/core"
    "${CMAKE_SOURCE_DIR}/core/abadia"
    "${CMAKE_SOURCE_DIR}/core/abadia/ui"
    "${CMAKE_SOURCE_DIR}/core/util"
    "${CMAKE_SOURCE_DIR}/core/systems"
    "${CMAKE_SOURCE_DIR}/platform/sdl2"
    "${CMAKE_SOURCE_DIR}/platform/sdl2/video"
    "${CMAKE_SOURCE_DIR}/platform/sdl2/audio"
    "${CMAKE_SOURCE_DIR}/platform/sdl2/input"
)

# Output location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/VigasocoSDL)
set_target_properties(vigasoco PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/VigasocoSDL"
)

# Copy assets folder to build output directory
add_custom_command(
    TARGET vigasoco POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/VigasocoSDL"
    COMMENT "Copying assets to build directory"
)