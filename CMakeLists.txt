cmake_minimum_required(VERSION 3.20)
project(jatraEngine2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system audio)

add_executable(jatraEngine2d main.cpp)

target_link_libraries(jatraEngine2d PRIVATE
        sfml-graphics sfml-window sfml-system sfml-audio
)

# Copy the whole assets/ dir after build
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:jatraEngine2d>/assets

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:jatraEngine2d>/resources
)

add_dependencies(jatraEngine2d copy_assets)

# Nice warnings with Clang/GCC
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(jatraEngine2d PRIVATE -Wall -Wextra -Wpedantic)
endif()
