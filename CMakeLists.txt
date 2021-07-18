cmake_minimum_required(VERSION 3.12)

# Using clang instead of gcc due to its Bug 85282, until v10.3.0
# Bug 85282 - CWG 727 (full specialization in non-namespace scope)
# NOTE: We should avoid set compiler directly in CMakeLists.txt!
# Instead, we could just call -DCMAKE_CXX_COMPILER=clang++ when
# invoking cmake, but since this project is just for studying
# purposes, there is no problem in forcing to use clang and
# avoid errors, since we are not going to use gcc anyway
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Setup our project
project(PhotoBatch)
set(SRC_DIR "src")

# Header files are not required, but it's a good practice
add_executable(
    photobatch
    "${SRC_DIR}/PhotoBatch.cpp"
    "${SRC_DIR}/ArgumentParser.h"
    "${SRC_DIR}/ArgumentParser.cpp"
    "${SRC_DIR}/Utils.h"
    "${SRC_DIR}/Utils.cpp"
    "${SRC_DIR}/Mode.h"
    "${SRC_DIR}/Mode.cpp"
    "${SRC_DIR}/RenameMode.h"
    "${SRC_DIR}/RenameMode.cpp"
    "${SRC_DIR}/ConvertMode.h"
    "${SRC_DIR}/ConvertMode.cpp"
    "${SRC_DIR}/ResizeMode.h"
    "${SRC_DIR}/ResizeMode.cpp"
)

# Setting the C++ standard that will be used in our target
# In this case, we could achieve the same thing as following:
# set_property(TARGET photobatch PROPERTY CXX_STANDARD 17)
# But we are going to use a specialized form of it instead
set_target_properties(photobatch PROPERTIES CXX_STANDARD 17)

# Use GTest
add_subdirectory(tests)

# Use stb
add_subdirectory(external)
target_link_libraries(photobatch PRIVATE stb)
