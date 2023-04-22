cmake_minimum_required(VERSION 3.24)
set(TARGET_NAME "foxy_utils")
set(TARGET_NAME_ALIAS "foxy::utils")
message(STATUS "Configuring ${PROJECT_NAME} sub-project: ${TARGET_NAME}")

# ===================================================
# CONFIGURATION
# ===================================================
option(FOXY_RELEASE_MODE_FILE_LOGGING "Log output to file even in release mode" OFF)

# ===================================================
# LIBRARY
# ===================================================
set(SOURCE_FILES
    "foxy_utils/types/foxy_types.ixx"
    "foxy_utils/types/packed_map.ixx"
    "foxy_utils/types/reference_count.ixx"
    "foxy_utils/types/singleton.ixx"
    "foxy_utils/time/foxy_time.ixx"
    "foxy_utils/time/stopwatch.ixx"
    "foxy_utils/time/game_loop.ixx"
    "foxy_utils/time/benchmark.ixx"
    "foxy_utils/time/time.ixx"
    "foxy_utils/io/foxy_io.ixx"
    "foxy_utils/io/byte.ixx"
    "foxy_utils/io/file.ixx"
    "foxy_utils/control_flow/foxy_flow.ixx"
    "foxy_utils/control_flow/for_each.ixx"
    "foxy_utils/ranges/foxy_ranges.ixx"
    "foxy_utils/ranges/contains.ixx"
    "foxy_utils/foxy_log.ixx"
    "foxy_utils/foxy_helpers.ixx"
    "foxy_utils/foxy_event.ixx"
    "foxy_utils/foxy_error.ixx"
    "foxy_utils/foxy_main.ixx"
    )
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
add_library(${TARGET_NAME_ALIAS} ALIAS ${TARGET_NAME})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_23)
target_compile_options(${TARGET_NAME} PRIVATE "/std:c++latest" "/experimental:module")
target_compile_definitions(${TARGET_NAME}
  PRIVATE _CRT_SECURE_NO_WARNINGS=1
         WIN32_LEAN_AND_MEAN=1
  PUBLIC NOMINMAX=1
        $<$<CONFIG:debug>:FOXY_ENABLE_ASSERTS=1;FOXY_DEBUG_MODE=1>
        $<$<CONFIG:relwithdebinfo>:FOXY_ENABLE_ASSERTS=1;FOXY_DEBUG_MODE=1;FOXY_RELDEB_MODE=1>
        $<$<CONFIG:release>:FOXY_ENABLE_ASSERTS=1;FOXY_RELEASE_MODE=1>
)
if(FOXY_RELEASE_MODE_FILE_LOGGING)
  target_compile_definitions(${TARGET_NAME} PUBLIC FOXY_RELEASE_MODE_FILE_LOGGING=1)
endif()
target_include_directories(${TARGET_NAME} PUBLIC "../include" ".")

# ===================================================
# DEPENDENCIES
# ===================================================
# STD
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC "../extern/std_library/include")
# SPDLOG
target_include_directories(${TARGET_NAME} SYSTEM PRIVATE "../extern/spdlog/include")
target_link_libraries(${TARGET_NAME} PRIVATE spdlog::spdlog)
# GLM
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC "../extern/glm")
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)
# UUID
target_compile_definitions(${TARGET_NAME} PUBLIC UUID_SYSTEM_GENERATOR=1)
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC "../extern/stduuid/include")
# JSON
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC "../extern/json/single_include")
target_link_libraries(${TARGET_NAME} PUBLIC nlohmann_json::nlohmann_json)
