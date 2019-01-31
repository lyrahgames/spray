cmake_minimum_required(VERSION 3.10)
project(spray-tests)
enable_testing()

find_package(doctest REQUIRED)
if (NOT TARGET spray::spray)
  find_package(spray CONFIG REQUIRED)
endif()

add_executable(${PROJECT_NAME}
  doctest_main.cc
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    doctest::doctest
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})