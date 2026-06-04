file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

add_library(engine_core STATIC ${ENGINE_SOURCES})

target_include_directories(engine_core PUBLIC include ../external/entt/src)

target_link_libraries(engine_core PUBLIC glm ImGui assimp)
