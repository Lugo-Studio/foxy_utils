cmake_minimum_required(VERSION 3.24)
set(TARGET_NAME example)

set(SOURCES "main.cpp")
add_executable(${TARGET_NAME} ${SOURCES})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_23)
target_compile_options(${TARGET_NAME} PRIVATE "/std:c++latest" "/experimental:module")
target_include_directories(${TARGET_NAME} PUBLIC ".")

# Koyote
target_include_directories(${TARGET_NAME} PUBLIC "../include")
target_link_libraries(${TARGET_NAME} PUBLIC foxy::utils)