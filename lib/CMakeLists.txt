include(sources.cmake)

add_subdirectory(platforms)

add_library(watchme_lib ${SOURCES})
add_library(Watchme::Lib ALIAS watchme_lib)

target_include_directories(watchme_lib PUBLIC inc PRIVATE src)
target_link_libraries(watchme_lib PRIVATE Watchme::Lib::Platform ImGuiLib)