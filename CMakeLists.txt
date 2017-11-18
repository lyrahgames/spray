cmake_minimum_required(VERSION 3.7.2)

project(spray VERSION 0.1)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(spray src/main.cpp)
target_link_libraries(spray Eigen3::Eigen)