file(GLOB TOOL_SOURCE "*.cc")

include_directories("../../engine/include")

add_executable(mesher ${TOOL_SOURCE})
#target_link_libraries(mesher engine)
set_target_properties(mesher PROPERTIES
    OUTPUT_NAME "mesher"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")