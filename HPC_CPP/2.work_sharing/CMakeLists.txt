cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

add_executable(Main main.cpp)

# cxx versioning
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

# link to openmp package
# note that on MacOS LibOMP needs to be installed via Homebrew
find_package(OPENMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Main PUBLIC OpenMP::OpenMP_CXX)
endif()