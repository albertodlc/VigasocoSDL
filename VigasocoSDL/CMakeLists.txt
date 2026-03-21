# 1. Grab everything first
file(GLOB ALL_ENGINE_SOURCES "*.cpp")

# 2. Exclude platform-specific files
set(PLATFORM_SPECIFIC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/PS2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDLThread.PS2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VigasocoSDL_PNACL.cpp"
)
list(REMOVE_ITEM ALL_ENGINE_SOURCES ${PLATFORM_SPECIFIC_FILES})

# 3. Create executable
add_executable(vigasoco ${ALL_ENGINE_SOURCES})

# 4. Link core library and SDL2
target_link_libraries(vigasoco PRIVATE vigasoco_core ${SDL2_LIBRARIES} dl)

# 5. Include paths
target_include_directories(vigasoco PRIVATE
    "${CMAKE_SOURCE_DIR}/core"
    "${CMAKE_SOURCE_DIR}/core/abadia"
    "${CMAKE_SOURCE_DIR}/core/util"
    "${CMAKE_SOURCE_DIR}/core/systems"
)

# 6. Output location
set_target_properties(vigasoco PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/VigasocoSDL"
)