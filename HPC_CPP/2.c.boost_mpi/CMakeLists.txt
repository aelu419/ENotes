cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

add_executable(Main main.cpp person.h)

target_compile_features(Main PUBLIC cxx_std_17)

# Boost is used for serializing custom classes
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# note:
#   Boost MPI needs to be specially built, check if your boost root has
#   some *-config.jam file, and add "using mpi ;" to it, then rebuild Boost
#   (see Boost_CPP for details)
find_package(Boost REQUIRED COMPONENTS serialization mpi)
target_include_directories(Main PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(Main PUBLIC ${Boost_LIBRARIES})

# note (Mac)
#   to solve the "A system call failed during shared memory initialization that should
#   not have.  It is likely that your MPI job will now either abort or
#   experience performance degradation." issue, run "export TMPDIR=/tmp"