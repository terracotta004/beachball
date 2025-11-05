cmake_minimum_required(VERSION 3.16)
project(rolling_beach_ball CXX)

# Use C++17 (safe for SFML 3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Let CMake know you’re using vcpkg (only needed if not passed via command line)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/Users/javer/source/repos/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# --- Find SFML 3 ---
# Note: Components use PascalCase in SFML 3
find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

# --- Build your executable ---
add_executable(rolling_beach_ball main.cpp)

# --- Link SFML libraries ---
target_link_libraries(rolling_beach_ball PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)