file(GLOB ENGINE_SOURCE "source/*.cc")

include_directories(
    "include"
    "../3rd-party/glm-0.9.7.1"
    "../3rd-party/glew-1.13.0/include")
add_library(engine SHARED ${ENGINE_SOURCE})
target_link_libraries(engine GLEW SDL2)