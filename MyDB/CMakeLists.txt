cmake_minimum_required(VERSION 3.14)
project(MyDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(include)

# Find all source files recursively (excluding main.cpp for the library)
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Create a static library containing all DB components
add_library(mydb_lib STATIC ${SRC_FILES})

# Main executable
add_executable(mydb src/main.cpp)
target_link_libraries(mydb PRIVATE mydb_lib)

# Example executable
# add_executable(example_basic examples/basic.cpp)
# target_link_libraries(example_basic PRIVATE mydb_lib)
