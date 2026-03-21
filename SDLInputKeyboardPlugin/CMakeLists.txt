# 1. Automatically find all source and header files in this folder
file(GLOB PLUGIN_SOURCES "*.cpp" "*.h")

# 2. Check if we actually found files before trying to create the library
if(NOT PLUGIN_SOURCES)
    message(FATAL_ERROR "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}!")
endif()

# 3. Create the plugin
add_library(SDLInputKeyboardPlugin MODULE ${PLUGIN_SOURCES})

# 4. Link against SDL (Note: Use the variable found by find_package)
target_link_libraries(SDLInputKeyboardPlugin PRIVATE ${SDL_LIBRARIES})

# 5. Set output directory
set_target_properties(SDLInputKeyboardPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)