file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "../backend/src/opengl_postprocessing_effects.cpp")

add_library(engine_core STATIC ${ENGINE_SOURCES})

target_include_directories(engine_core PUBLIC include ../external/entt/src ../external/glad/include)

target_link_libraries(engine_core PUBLIC glm ImGui assimp)
