cmake_minimum_required(VERSION 3.10)
project(App VERSION 0.1.0)

# add the target first (note order matters)
add_executable(App app.cxx)

# pass in version number from the header file
configure_file(Config.h.in Config.h)
target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}")

# specify C++ standard
# note:
#   - CXX instead of CPP
#   - capitalize boolean
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)