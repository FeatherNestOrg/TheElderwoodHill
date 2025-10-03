add_executable(${PROJECT_NAME}
        main.cpp
        Game.cpp
        Map/Map.cpp
        Map/Animation.cpp
        Map/Renderer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src")

target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        tl::expected
        tmxparser::tmxparser
        unordered_dense::unordered_dense
)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif ()
