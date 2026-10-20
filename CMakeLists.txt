cmake_minimum_required(VERSION 3.15)
project(VDaniKex VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(VDaniKexCore SHARED
    src/core.cpp
)
target_include_directories(VDaniKexCore PUBLIC include)
target_compile_definitions(VDaniKexCore PRIVATE VDANIKEX_BUILD_DLL WIN32_LEAN_AND_MEAN NOMINMAX)

add_executable(vdk-probe src/probe.cpp)
target_include_directories(vdk-probe PRIVATE include)
target_compile_definitions(vdk-probe PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)

if(MSVC)
    target_compile_options(VDaniKexCore PRIVATE /W4 /permissive-)
    target_compile_options(vdk-probe PRIVATE /W4 /permissive-)
else()
    target_compile_options(VDaniKexCore PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vdk-probe PRIVATE -Wall -Wextra -Wpedantic)
endif()
