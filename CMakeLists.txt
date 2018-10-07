project(zerk3d)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wfatal-errors -pedantic -std=gnu99 -Wl,--no-undefined -fPIC -Wall -Wextra -Wconversion -Werror=return-type -Werror=implicit-function-declaration")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -pedantic -std=c++11 -Wl,--no-undefined -fPIC -Wall -Wextra -Wconversion -Werror=return-type -Werror=implicit-function-declaration")

add_subdirectory("engine")
add_subdirectory("game")
add_subdirectory("tools/mesher")

#add_definitions(-DNEW_RENDER)
#add_executable(zerk3d "source/video.cc" "source/textured.cpp" "source/extra.cc")
#target_link_libraries(zerk3d "SDL2")