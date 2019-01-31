set(pkgconfig_install_dir "lib/pkgconfig")

configure_file(
  cmake/${PROJECT_NAME}.pc.in
  ${PROJECT_NAME}.pc
  @ONLY
)
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  DESTINATION
    ${pkgconfig_install_dir}
)