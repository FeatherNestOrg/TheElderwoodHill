set(TEH_ASSETS_PATH "${PROJECT_SOURCE_DIR}/assets/")
target_compile_definitions(${PROJECT_NAME} PRIVATE
        TEH_ASSETS_PATH="${TEH_ASSETS_PATH}"
)