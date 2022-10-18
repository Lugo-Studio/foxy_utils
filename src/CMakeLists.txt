cmake_minimum_required(VERSION 3.24)
set(TARGET_NAME "koyote")
message(STATUS "Configuring ${PROJECT_NAME} sub-project: ${TARGET_NAME}")

# ===================================================
# CONFIGURATION
# ===================================================
option(FOXY_RELEASE_MODE_FILE_LOGGING "Log output to file even in release mode" OFF)

# ===================================================
# LIBRARY
# ===================================================
set(SOURCE_FILES
    "koyote/log.cpp"
    "koyote/time.cpp"
    "koyote/file.cpp"
    )
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
add_library(${TARGET_NAME}::utils ALIAS ${TARGET_NAME})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_23)
target_compile_options(${TARGET_NAME} PRIVATE "/bigobj" "/std:c++latest" "/experimental:module")
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
target_precompile_headers(${TARGET_NAME} PUBLIC "koyote/pch.hpp")
target_include_directories(${TARGET_NAME} PUBLIC "../include" ".")

# ===================================================
# DEPENDENCIES
# ===================================================
# SPDLOG
target_include_directories(${TARGET_NAME} SYSTEM PRIVATE "../extern/spdlog/include")
target_link_libraries(${TARGET_NAME} PRIVATE spdlog::spdlog)
# GLM
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC "../extern/glm")
target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)
# UUID
target_compile_definitions(${TARGET_NAME} PUBLIC UUID_SYSTEM_GENERATOR=1)
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC "../extern/stduuid/include")
# JSON
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC "../extern/json/include")
target_link_libraries(${TARGET_NAME} PUBLIC nlohmann_json::nlohmann_json)
