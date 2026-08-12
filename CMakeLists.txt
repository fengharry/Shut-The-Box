cmake_minimum_required(VERSION 3.2)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project description
project(
    Shut_The_Box
    VERSION 1.0
    DESCRIPTION "Shut the Box"
    LANGUAGES CXX)

# Process the src and apps directories
add_subdirectory(ShutTheBox)
add_subdirectory(Strategies)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC ShutTheBox)
target_link_libraries(main PUBLIC Strategies)

# Force the generation of a compile_commands.json file to provide autocompletion for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE BOOL "" FORCE)