cmake_minimum_required(VERSION 3.21)
project(VirtualEngineECU LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32 OR NOT MSVC)
    message(FATAL_ERROR "This demo targets Windows with the MSVC compiler.")
endif()

set(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/FreeRTOS-Kernel"
    CACHE PATH "Path to the FreeRTOS-Kernel checkout")

if(NOT EXISTS "${FREERTOS_KERNEL_PATH}/tasks.c")
    message(FATAL_ERROR
        "FreeRTOS-Kernel is missing. Run: git submodule update --init --recursive")
endif()

add_executable(VirtualEngineECU
    src/main.cpp
    src/Tasks.cpp
    src/Drivers.cpp
    src/HardwareSim.cpp
    "${FREERTOS_KERNEL_PATH}/tasks.c"
    "${FREERTOS_KERNEL_PATH}/queue.c"
    "${FREERTOS_KERNEL_PATH}/list.c"
    "${FREERTOS_KERNEL_PATH}/portable/MSVC-MingW/port.c"
    "${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c"
)

target_include_directories(VirtualEngineECU PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${FREERTOS_KERNEL_PATH}/include"
    "${FREERTOS_KERNEL_PATH}/portable/MSVC-MingW"
)

target_compile_definitions(VirtualEngineECU PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(VirtualEngineECU PRIVATE winmm)

if(MSVC)
    target_compile_options(VirtualEngineECU PRIVATE /W4 /permissive-)
endif()
