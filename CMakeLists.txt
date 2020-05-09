cmake_minimum_required(VERSION 3.17)

project(WatchMe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/thirdparties/imgui.cmake)


add_subdirectory(lib)
add_subdirectory(samples)
add_subdirectory(tests)

include(cmake/clangformat.cmake)