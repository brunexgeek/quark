project(zerk3d)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wfatal-errors")

add_subdirectory("engine")
add_subdirectory("game")

#add_definitions(-DNEW_RENDER)
#add_executable(zerk3d "source/video.cc" "source/textured.cpp" "source/extra.cc")
#target_link_libraries(zerk3d "SDL2")