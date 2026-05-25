file(GLOB_RECURSE APP_SOURCES "src/*.cpp")

add_executable(engine ${APP_SOURCES})

target_link_libraries(engine PRIVATE engine_core graphic_backend glfw)

add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
)
