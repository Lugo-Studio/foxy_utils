cmake_minimum_required(VERSION 3.24)
set(TARGET_NAME "koyote")
message(STATUS "Configuring ${PROJECT_NAME} sub-project: ${TARGET_NAME}")

# ===================================================
# LIBRARY
# ===================================================
set(SOURCE_FILES
    "koyote/core/std.cpp"
    "koyote/core/log.cpp"
    "koyote/core/event.cpp"
    "koyote/core/time.cpp"
    "koyote/io/file.cpp"
    )
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_23)
target_compile_options(${TARGET_NAME} PRIVATE "/bigobj" "/std:c++latest" "/experimental:module")
target_compile_definitions(${TARGET_NAME}
                           PRIVATE _CRT_SECURE_NO_WARNINGS=1
                                   WIN32_LEAN_AND_MEAN=1
                           PUBLIC NOMINMAX=1
                                  $<$<CONFIG:debug>:FOXY_ENABLE_ASSERTS=1;FOXY_DEBUG_MODE=1>
                                  $<$<CONFIG:relwithdebinfo>:FOXY_ENABLE_ASSERTS=1;FOXY_DEBUG_MODE=1;FOXY_RELDEB_MODE=1>
                           )
target_precompile_headers(${TARGET_NAME} PUBLIC "koyote/internal/pch.hpp")
target_include_directories(${TARGET_NAME} PUBLIC "../include" ".")

# ===================================================
# DEPENDENCIES
# ===================================================
# SPDLOG
target_include_directories(${TARGET_NAME} PRIVATE "../extern/spdlog/include")
target_link_libraries(${TARGET_NAME} PRIVATE spdlog::spdlog)
# GLM
target_compile_definitions(${TARGET_NAME} PUBLIC GLM_FORCE_RADIANS=1)
target_include_directories(${TARGET_NAME} PUBLIC "../extern/glm")
target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)
# UUID_v4
target_include_directories(${TARGET_NAME} PUBLIC "../extern/uuid_v4")
