cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

add_executable(Main main.cpp person.h)

target_compile_features(Main PUBLIC cxx_std_17)

# link to openmpi package
# note that on MacOS Open-MPI needs to be installed via Homebrew
find_package(MPI REQUIRED)
if(MPI_CXX_FOUND)
    target_link_libraries(Main PUBLIC MPI::MPI_CXX)
endif()

# Boost is used for serializing custom classes
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS serialization)
target_include_directories(Main PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(Main PUBLIC Boost::serialization)