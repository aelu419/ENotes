cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

add_executable(Main main.cpp)
target_compile_features(Main PUBLIC cxx_std_17)

find_package(MPI REQUIRED)
if(MPI_CXX_FOUND)
    target_link_libraries(Main PUBLIC MPI::MPI_CXX)
endif()