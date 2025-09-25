set(BUILD_TESTS OFF)
FetchContent_Declare(
        tileson_fetched
        GIT_REPOSITORY https://github.com/SSBMTonberry/tileson.git
        GIT_TAG "master"
)
FetchContent_MakeAvailable(tileson_fetched)

# 为Tileson单头文件库创建INTERFACE target
add_library(tileson INTERFACE)
target_include_directories(tileson INTERFACE ${tileson_fetched_SOURCE_DIR})

# 创建别名以保持一致的命名风格
add_library(tileson::tileson ALIAS tileson)
