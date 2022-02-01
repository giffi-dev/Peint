cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_C_COMPILER /usr/bin/gcc)
#set(CMAKE_C_COMPILER /usr/bin/clang)

project("Peint" LANGUAGES C)

# Main Programin
add_executable(${PROJECT_NAME}
    src/main.c
    src/canvas.c
    src/editor.c
    src/tools.c
    src/undo.c
)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

# Libs
add_subdirectory(thirdparty/raylib)
add_subdirectory(thirdparty/raygui)
add_subdirectory(thirdparty/tinyfiledialogs)

target_include_directories(${PROJECT_NAME} 
    PRIVATE thirdparty/raylib/src
    PRIVATE thirdparty/raygui
    PRIVATE thirdparty/tinyfiledialogs
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE thirdparty/raylib/src
    PRIVATE thirdparty/raygui
    PRIVATE thirdparty/tinyfiledialogs
)

target_link_libraries(${PROJECT_NAME} raylib raygui tinyfiledialogs)

