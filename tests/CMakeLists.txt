# We'd like to see some logs when fetching our dependencies
set(FETCHCONTENT_QUIET OFF CACHE BOOL "USE FETCHCONTENT_QUIET" FORCE)

# Always use msvcrt.dll, this way it will "just work"
set(gtest_force_shared_crt ON CACHE BOOL "USE MSVCRT.DLL" FORCE)

# Use FetchContent
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.10.0
)

# All done, let's fetch
FetchContent_MakeAvailable(googletest)

# For all intents and purposes, we should encapsulate our code units
# in libraries/modules, including but not limited to save time below
# But for the sake of simplicity, let's just compile it twice
add_executable(
    TestArgumentParser
    "TestArgumentParser.cpp"
    "${CMAKE_SOURCE_DIR}/${SRC_DIR}/ArgumentParser.cpp"
    "${CMAKE_SOURCE_DIR}/${SRC_DIR}/Utils.cpp"
)

# Since we are using headers of our actual code inside tests,
# we must tell CMake where to find them, otherwise it won't compile
target_include_directories(
    TestArgumentParser PRIVATE "${CMAKE_SOURCE_DIR}/${SRC_DIR}"
)

# All done, now let's just link our test with gtest lib
target_link_libraries(TestArgumentParser PRIVATE gtest_main)
