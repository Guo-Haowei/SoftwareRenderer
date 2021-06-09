cmake_minimum_required(VERSION 3.0)
project(SoftwareRenderer)
set(CMAKE_CXX_STANDARD 17)

# glm
message("Configuring glm...")
include_directories(external/glm/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(source)
