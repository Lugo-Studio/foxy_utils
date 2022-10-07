cmake_minimum_required(VERSION 3.24)
set(TARGET_NAME "koyote")
#set(CMAKE_CXX_STANDARD 23)
message(STATUS "Configuring ${PROJECT_NAME} sub-project: ${TARGET_NAME}")

# ===================================================
# LIBRARY
# ===================================================
set(SOURCE_FILES
    "koyote/core/std.cpp"
    "koyote/core/log.cpp"
    "koyote/core/event.cpp"
    "koyote/core/ring_buffer.cpp"
    "koyote/core/time.cpp")
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
target_include_directories(${TARGET_NAME} PUBLIC ".")

# ===================================================
# DEPENDENCIES
# ===================================================
# SPDLOG
target_include_directories(${TARGET_NAME} PRIVATE "${FOXY_EXTERN_DIR}/spdlog/include")
target_link_libraries(${TARGET_NAME} PRIVATE spdlog::spdlog)
# include(ExternalProject)
# set(SPDLOG_ENABLE_PCH ON)
# ExternalProject_Add(spdlog
#     PREFIX spdlog
#     SOURCE_DIR ${FOXY_EXTERN_DIR}/spdlog
#     CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#     -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#     -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#     -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
#     -DCMAKE_INSTALL_PREFIX=${STAGING_DIR}
#     -DSPDLOG_BUILD_SHARED=OFF
# )
# add_dependencies(${TARGET_NAME} spdlog)
# target_include_directories(${TARGET_NAME} PRIVATE "${FOXY_EXTERN_DIR}/include")
# GLM
target_compile_definitions(${TARGET_NAME} PUBLIC GLM_FORCE_RADIANS=1)
target_include_directories(${TARGET_NAME} PUBLIC "${FOXY_EXTERN_DIR}/glm")
target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)
# UUID_v4
target_include_directories(${TARGET_NAME} PUBLIC "${FOXY_EXTERN_DIR}/uuid_v4")
