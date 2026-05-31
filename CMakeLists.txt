cmake_minimum_required(VERSION 3.16)
project(originium_circuit_repair CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(raylib 5.5 QUIET)
if(NOT raylib_FOUND)
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    FetchContent_MakeAvailable(raylib)
endif()

add_executable(game
    src/main.cpp
    src/core/Part.cpp
    src/core/Board.cpp
    src/core/Parser.cpp
    src/core/WinChecker.cpp
    src/core/Solver.cpp
    src/core/Game.cpp
    src/ui/Input.cpp
    src/ui/Renderer.cpp
)

target_include_directories(game PRIVATE src)
target_link_libraries(game PRIVATE raylib)

if(WIN32 AND MINGW)
    target_link_options(game PRIVATE -static -static-libgcc -static-libstdc++)
endif()

# Copy assets/ next to the built executable so runtime relative paths work.
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:game>/assets)
