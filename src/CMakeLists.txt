add_executable(${PROJECT_NAME}
        main.cpp
        Game.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3::SDL3
        tl::expected
        tinytmx::tinytmx
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()
