# 1. Grab all .cpp files
file(GLOB ALL_CPP "*.cpp")

# 2. Filter out the Template implementation file 
# because it is already included by the .h file
list(REMOVE_ITEM ALL_CPP "${CMAKE_CURRENT_SOURCE_DIR}/SDLBasicDrawPluginTemplates.cpp")

# 3. Define the plugin with the filtered list
add_library(SDLVideoPlugin MODULE ${ALL_CPP})

# ... rest of your file (include_directories, link_libraries, etc)
target_include_directories(SDLVideoPlugin PRIVATE "${CMAKE_SOURCE_DIR}/core")
target_link_libraries(SDLVideoPlugin PRIVATE ${SDL_LIBRARIES})

set_target_properties(SDLVideoPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)