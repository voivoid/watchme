include(sources.cmake)

add_library(watchme_lib_platform ${SOURCES})
add_library(Watchme::Lib::Platform ALIAS watchme_lib_platform)

target_link_libraries(watchme_lib_platform PRIVATE ImGui::Platform)
target_include_directories(watchme_lib_platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(watchme_lib_platform PUBLIC UNICODE _UNICODE)
