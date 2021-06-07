cmake_minimum_required(VERSION 3.0)
project(SoftwareRenderer)
set(CMAKE_CXX_STANDARD 17)

# glfw
message("Configuring glfw...")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
include_directories(external/glfw/include)
add_subdirectory(external/glfw)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(source)
