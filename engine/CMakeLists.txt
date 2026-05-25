file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

add_library(engine_core STATIC ${ENGINE_SOURCES})

target_include_directories(engine_core PUBLIC include external)

file(GLOB IMGUI_SOURCES "external/imgui/*.cpp")
target_sources(engine_core PRIVATE ${ENGINE_SOURCES} ${IMGUI_SOURCES})

target_link_libraries(engine_core PUBLIC glm)
