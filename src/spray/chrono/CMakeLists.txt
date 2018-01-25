cmake_minimum_required(VERSION 3.9)

project(spray.chrono VERSION 0.1)

add_library(spray.chrono STATIC
  fps_meter.cc
)

set_property(TARGET spray.chrono PROPERTY CXX_STANDARD 14)

add_library(spray::chrono ALIAS spray.chrono)