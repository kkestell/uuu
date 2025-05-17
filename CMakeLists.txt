cmake_minimum_required(VERSION 3.10)
project(uuu C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find SDL2
find_package(SDL2 REQUIRED)

# Use pkg-config for assimp and cglm
include(FindPkgConfig)
pkg_check_modules(ASSIMP REQUIRED assimp)
pkg_check_modules(CGLM REQUIRED cglm)

# Set source files
set(SOURCES
        src/main.c
        src/mesh.c
        src/window.c
        src/renderer.c
        src/platform.c
        src/math.c
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        ${ASSIMP_INCLUDE_DIRS}
        ${CGLM_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2::SDL2
        ${ASSIMP_LIBRARIES}
        ${CGLM_LIBRARIES}
        m
)

# Link directories if needed
link_directories(
        ${ASSIMP_LIBRARY_DIRS}
        ${CGLM_LIBRARY_DIRS}
)

# Enable debug symbols in Debug build
target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:-g>")

# Set output directory
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)