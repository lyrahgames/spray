cmake_minimum_required(VERSION 3.9)

project(spray VERSION 0.1)

add_subdirectory(ray_tracer)
add_subdirectory(chrono)

add_library(spray INTERFACE)
target_include_directories(spray INTERFACE ..)
target_link_libraries(spray INTERFACE spray::ray_tracer spray::chrono)

add_library(spray::spray ALIAS spray)