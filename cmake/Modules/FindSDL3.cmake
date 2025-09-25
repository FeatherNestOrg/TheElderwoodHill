find_package(SDL3 QUIET)

if (NOT SDL3_FOUND)
    message(STATUS "SDL3 can't be found locally, try fetching from remote...")
    FetchContent_Declare(
            SDL3_fetched
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG "main"
    )
    FetchContent_MakeAvailable(SDL3_fetched)
else ()
    message(STATUS "Found SDL3 v${SDL3_VERSION}")
endif ()