# We'd like to see some logs when fetching our dependencies
set(FETCHCONTENT_QUIET OFF CACHE BOOL "USE FETCHCONTENT_QUIET" FORCE)

# Use FetchContent
include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
)

# All done, let's fetch
FetchContent_MakeAvailable(stb)

# Get fetched source directory into a variable
FetchContent_GetProperties(stb SOURCE_DIR STB_SRC)

# Since stb has no CMakeLists, we need to add the lib ourselves
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${STB_SRC})
