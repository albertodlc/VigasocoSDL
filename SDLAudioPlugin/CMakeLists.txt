# Collect all .cpp files in this folder automatically
file(GLOB PLUGIN_SOURCES "*.cpp")

# Define this as a "Module" (a plugin)
add_library(SDLAudioPlugin MODULE ${PLUGIN_SOURCES})

# Link against SDL
target_link_libraries(SDLAudioPlugin PRIVATE ${SDL_LIBRARY})

# Tell CMake to output the .so file directly into our bin/plugins folder
set_target_properties(SDLAudioPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)