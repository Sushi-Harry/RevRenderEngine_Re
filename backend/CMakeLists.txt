file(GLOB_RECURSE BACKEND_SOURCES "src/*.cpp")

add_library(graphic_backend STATIC
    ${BACKEND_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/src/glad.c"
)

target_include_directories(graphic_backend PUBLIC
    include
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external"
)

target_link_libraries(graphic_backend PUBLIC engine_core glm)

if(TARGET glfw)
    target_link_libraries(graphic_backend PUBLIC glfw)
else()
    target_link_libraries(graphic_backend PUBLIC glfw::glfw)
endif()
