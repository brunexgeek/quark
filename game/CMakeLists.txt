file(GLOB GAME_SOURCE "source/*.cc")

include_directories(
    "../engine/include"
    "../3rd-party/glew-1.13.0/include")

add_executable(zerk3d ${GAME_SOURCE})
target_link_libraries(zerk3d engine)
set_target_properties(zerk3d PROPERTIES
    OUTPUT_NAME "zerk3d"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")