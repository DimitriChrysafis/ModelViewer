cmake_minimum_required(VERSION 3.10)
project(PlayerGROUND)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.6 REQUIRED graphics window system)

add_executable(PlayerGROUND main.cpp
        model_viewer.hpp
        model_viewer.cpp
        convex_hull.hpp
        camera.cpp
        convex_hull.cpp
        kdtree.hpp
        kdtree.cpp
)

target_link_libraries(PlayerGROUND sfml-graphics sfml-window sfml-system)
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/demos)
file(COPY ${RESOURCE_DIR}/cow.obj DESTINATION ${CMAKE_BINARY_DIR})