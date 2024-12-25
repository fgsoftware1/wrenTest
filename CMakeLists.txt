cmake_minimum_required(VERSION 3.31.3)

project(wrenTest LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/wrenbind/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_directories(
    ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/lib
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Wren)