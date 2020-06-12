find_program(ClangFormat clang-format)

if(ClangFormat)

  set(Targets watchme_lib watchme_sample watchme_lib_platform)
  foreach(Target ${Targets})
    get_target_property(TargetSources ${Target} SOURCES)
    get_target_property(TargetDir ${Target} SOURCE_DIR)
    foreach(Src ${TargetSources})
      list(APPEND Sources "${TargetDir}/${Src}")
    endforeach()
  endforeach()

  list(FILTER Sources INCLUDE REGEX "^.*\.(h|cpp)\$")

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${Sources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif()
