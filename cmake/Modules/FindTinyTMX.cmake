if(WIN32)
    include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

FetchContent_Declare(
        tinytmx_fetched
        GIT_REPOSITORY https://github.com/FeatherNestOrg/tinytmx.git
        GIT_TAG "master"
)
FetchContent_MakeAvailable(tinytmx_fetched)