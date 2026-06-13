file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

add_library(engine_core STATIC ${ENGINE_SOURCES} ../external/glad/src/glad.c)

target_include_directories(engine_core PUBLIC include ../external/entt/src ../external/glad/include)

target_link_libraries(engine_core PUBLIC glm ImGui assimp)
