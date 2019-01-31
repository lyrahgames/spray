export(
  TARGETS ${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAMESPACE}::
  FILE ${targets_export_name}.cmake
)
export(PACKAGE ${PROJECT_NAME})
