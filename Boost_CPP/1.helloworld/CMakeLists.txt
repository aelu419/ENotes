cmake_minimum_required(VERSION 3.12)
project(Main VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Main main.cpp)

# to build, follow https://www.boost.org/doc/libs/1_79_0/more/getting_started/unix-variants.html#header-only-libraries
#   or the most up to date version for the platform
#   in addition to the provided commands `./b2`, `./b2 header`, also run `./b2 install` to install at `/usr/local`
#   to specify installation prefix, there are dedicated flags for b2

# while building...
# The Boost C++ Libraries were successfully built!

# The following directory should be added to compiler include paths:

#     /usr/local/boost_1_79_0

# The following directory should be added to linker library paths:

#     /usr/local/boost_1_79_0/stage/lib

# this finds the entire boost library
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# for components that DO need to be built, specify them here
find_package(Boost REQUIRED COMPONENTS filesystem regex)

# the headers-only libraries can just be included
# for example, the Array library is included below without actually mentioning it
target_include_directories(Main PUBLIC ${Boost_INCLUDE_DIRS})

message(STATUS "Boost version: ${Boost_VERSION}")

# message(STATUS "---------------------")
# message(STATUS "Boost_FOUND: ${Boost_FOUND}")
# message(STATUS "Boost version: ${Boost_VERSION}")
# message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
# message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
# message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
# message(STATUS "---------------------")

# link the built libraries
target_link_libraries(Main PUBLIC Boost::filesystem Boost::regex)