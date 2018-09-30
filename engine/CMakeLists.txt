file(GLOB ENGINE_SOURCE "source/*.cc")

include_directories(
    "include"
    "../3rd-party/glew-1.13.0/include")
add_library(engine STATIC ${ENGINE_SOURCE})
target_link_libraries(engine GLEW GL SDL2)