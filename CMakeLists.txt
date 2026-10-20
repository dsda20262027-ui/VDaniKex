cmake_minimum_required(VERSION 3.15)
project(VDaniKex VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(VDaniKexCore SHARED src/core.cpp)
target_include_directories(VDaniKexCore PUBLIC include)
target_compile_definitions(VDaniKexCore PRIVATE VDANIKEX_BUILD_DLL WIN32_LEAN_AND_MEAN NOMINMAX)

add_executable(vdk-probe src/probe.cpp)
target_include_directories(vdk-probe PRIVATE include)
target_compile_definitions(vdk-probe PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)

add_executable(vdk-vm src/vdk_vm.cpp)
target_compile_definitions(vdk-vm PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)

add_executable(vdk-manager WIN32 src/vdk_manager.cpp)
target_compile_definitions(vdk-manager PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(vdk-manager PRIVATE comdlg32)

if(MSVC)
    target_compile_options(VDaniKexCore PRIVATE /W4 /permissive-)
    target_compile_options(vdk-probe PRIVATE /W4 /permissive-)
    target_compile_options(vdk-vm PRIVATE /W4 /permissive-)
    target_compile_options(vdk-manager PRIVATE /W4 /permissive-)
    # This subsystem version is a baseline declaration, not proof of OS compatibility.
    target_link_options(vdk-probe PRIVATE /SUBSYSTEM:CONSOLE,6.01)
    target_link_options(vdk-vm PRIVATE /SUBSYSTEM:CONSOLE,6.01)
    target_link_options(vdk-manager PRIVATE /SUBSYSTEM:WINDOWS,6.01)
else()
    target_compile_options(VDaniKexCore PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vdk-probe PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vdk-vm PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vdk-manager PRIVATE -Wall -Wextra -Wpedantic)
endif()
