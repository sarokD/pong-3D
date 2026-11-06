cmake_minimum_required(VERSION 3.16)
project(3d-pong LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

file (GLOB SRCS CONFIGURE_DEPENDS src/*.c*)
file (GLOB HEADERS CONFIGURE_DEPENDS include/*.h*)

add_executable(3d-pong ${SRCS} ${HEADERS})
target_include_directories(3d-pong PRIVATE include)

if (MSVC)
    target_compile_options(3d-pong PRIVATE /W4)
else()
    target_compile_options(3d-pong PRIVATE -Wall -Wextra)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(3d-pong PRIVATE glfw OpenGL::GL)

if(UNIX AND NOT APPLE)
    target_link_libraries(3d-pong PRIVATE m)
endif()