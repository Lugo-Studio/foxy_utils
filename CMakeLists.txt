cmake_minimum_required(VERSION 3.24)
project(foxy_utils)

add_subdirectory(extern/glm)
set(SPDLOG_ENABLE_PCH ON)
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_WCHAR_SUPPORT ON)
add_subdirectory(extern/spdlog)
add_subdirectory(extern/json)

add_subdirectory(src)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/bin/$<CONFIG>/${CMAKE_SYSTEM_NAME}")
  if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE $<$<OR:$<CONFIG:release>,$<CONFIG:minsizerel>>:TRUE>)
  endif()
  
  add_subdirectory(example)
endif()
