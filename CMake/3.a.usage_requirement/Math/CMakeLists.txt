add_library(Math arithmetics.cxx)

# add to interface includes property, such that everything referencing the Math library will include this source directory
# note that this cancels out the $EXTRA_INCLUDES in the top level CMakeLists
target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})