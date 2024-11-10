cmake_minimum_required(VERSION 3.7)

project(raytracer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(src)
include_directories(src/ray_tracer)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h src/ray_tracer/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
