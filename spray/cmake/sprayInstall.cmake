set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(version_config "${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${PROJECT_NAME}Config.cmake") # this file is already given
set(targets_export_name "${PROJECT_NAME}Targets")

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${targets_export_name}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# install(
#   FILES hash_map.h
#   DESTINATION include/${PROJECT_NAME}
# )
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "tests" EXCLUDE
  PATTERN "cmake" EXCLUDE
)

install(
  EXPORT ${targets_export_name}
  NAMESPACE ${PROJECT_NAMESPACE}::
  DESTINATION ${config_install_dir}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  cmake/${project_config}.in
  ${project_config}
  INSTALL_DESTINATION ${config_install_dir}
)

write_basic_package_version_file(
  ${version_config}
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${project_config}
    ${CMAKE_CURRENT_BINARY_DIR}/${version_config}
  DESTINATION
    ${config_install_dir}
)