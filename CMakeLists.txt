cmake_minimum_required(VERSION 3.31)
project(TheElderwoodHill)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 26)

include(FetchModules)

add_subdirectory(src)