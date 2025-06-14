# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.30.8)

# Export compile commands (useful for tools like clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project information
project(ash VERSION 0.0.2 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files
set(SOURCES
  src/ash.cpp
  src/ash_execvp.cpp
  src/main.cpp
)

# Add header files (optional)
set(HEADERS
  include/ash_execvp.h
  include/ash.h
)

# Define compile flags
set(COMPILE_FLAGS -Wall)

# List libraries to link (if any)
set(LINK_LIBRARIES
)

# Function to simplify target creation
function(add_project_target TARGET_NAME SOURCES HEADERS LIBS)
    add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
    target_include_directories(${TARGET_NAME} PRIVATE include)
    target_compile_options(${TARGET_NAME} PRIVATE ${COMPILE_FLAGS})
    target_link_libraries(${TARGET_NAME} PRIVATE ${LIBS})
    
    # Specify the output directory for the binary
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    )
endfunction()

# Add the main project target
add_project_target(${PROJECT_NAME} "${SOURCES}" "${HEADERS}" "${LINK_LIBRARIES}")

# Optionally define build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Installation target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Additional configurations based on compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using GCC or Clang")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Using MSVC")
endif()
