cmake_minimum_required(VERSION 3.31)

if(WIN32)
    if(NOT DEFINED ENV{VCPKG_ROOT})
        message(FATAL_ERROR "Environment variable VCPKG_ROOT not set; please set it to your vcpkg root path.")
    endif()
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Vcpkg toolchain file")
endif ()

project(TheElderwoodHill)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 26)

include(FetchModules)

add_subdirectory(src)