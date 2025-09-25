set(BUILD_TESTS OFF)
FetchContent_Declare(
        tileson_fetched
        GIT_REPOSITORY https://github.com/SSBMTonberry/tileson.git
        GIT_TAG "master"
)
FetchContent_MakeAvailable(tileson_fetched)