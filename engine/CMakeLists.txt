file(GLOB ENGINE_SOURCE "source/*.cc")

add_library(engine STATIC ${ENGINE_SOURCE})
target_include_directories(engine
    PUBLIC "include"
    PRIVATE "../3rd-party/glew-1.13.0/include")
target_link_libraries(engine GLEW GL SDL2)