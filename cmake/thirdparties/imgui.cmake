include(FetchContent)

FetchContent_Declare(
  get_imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG        v1.76
)

FetchContent_GetProperties(get_imgui)
if(NOT get_imgui_POPULATED)
  FetchContent_Populate(get_imgui)

  set(SrcDir ${get_imgui_SOURCE_DIR})
  add_library(ImGuiLib ${SrcDir}/imconfig.h
                       ${SrcDir}/imgui.cpp
                       ${SrcDir}/imgui.h
                       ${SrcDir}/imgui_draw.cpp
                       ${SrcDir}/imgui_widgets.cpp
                       ${SrcDir}/imgui_internal.h
                       ${SrcDir}/imstb_rectpack.h
                       ${SrcDir}/imstb_textedit.h
                       ${SrcDir}/imstb_truetype.h)

  target_include_directories(ImGuiLib PUBLIC ${SrcDir})

  if(WIN32)
    add_library(ImGuiLibPlatform ${SrcDir}/examples/imgui_impl_win32.h
                                 ${SrcDir}/examples/imgui_impl_win32.cpp
                                 ${SrcDir}/examples/imgui_impl_dx9.h
                                 ${SrcDir}/examples/imgui_impl_dx9.cpp)
    target_link_libraries(ImGuiLibPlatform PUBLIC ImGuiLib d3d9.lib)
    target_include_directories(ImGuiLibPlatform PUBLIC ${SrcDir}/examples)
  else()
    message(ERROR "Unknown platform")
  endif()

endif()