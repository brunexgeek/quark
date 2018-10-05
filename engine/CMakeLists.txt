file(GLOB_RECURSE ENGINE_SOURCE "source/*.cc")

find_package(OpenGL)

add_library(engine STATIC ${ENGINE_SOURCE})
target_include_directories(engine
    PUBLIC "include"
    PRIVATE "../3rd-party/glew-1.13.0/include")
target_link_libraries(engine GLEW ${OPENGL_gl_LIBRARY} SDL2)