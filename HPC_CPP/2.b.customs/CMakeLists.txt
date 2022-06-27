cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

add_executable(Main main.cpp)

# cxx versioning
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

# link to openmpi package
# note that on MacOS Open-MPI needs to be installed via Homebrew
find_package(MPI REQUIRED)
if(MPI_CXX_FOUND)
    target_link_libraries(Main PUBLIC MPI::MPI_CXX)
endif()