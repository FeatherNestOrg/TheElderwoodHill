find_package(tmxparser QUIET)

if (NOT tmxparser_FOUND)
    FetchContent_Declare(tmxparser
            GIT_REPOSITORY https://github.com/120MF/tmxparser
            GIT_TAG master
    )
    FetchContent_MakeAvailable(tmxparser)
endif ()
