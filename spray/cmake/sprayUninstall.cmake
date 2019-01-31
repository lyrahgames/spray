configure_file(
  cmake/${PROJECT_NAME}Uninstall.cmake.in
  ${PROJECT_NAME}Uninstall.cmake
  @ONLY
)
add_custom_target(
  uninstall
  COMMAND
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Uninstall.cmake
)