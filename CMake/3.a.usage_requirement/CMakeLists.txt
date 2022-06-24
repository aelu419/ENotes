# usage requirement is a modern way of handeling dependencies
# this project is considered a refactor of the 2.a.library project

# see Math/CMakeLists.txt for more changes

cmake_minimum_required(VERSION 3.10)
project(App VERSION 0.1.0)

option(USE_MATH "Use custom math implementation" ON)
configure_file(Config.h.in Config.h)

# moved target to above USE_MATH condition
# this way App can be linked to Math directly within the condition, without the use of EXTRA_LIBS
add_executable(App app.cxx)

if(USE_MATH)
    add_subdirectory(Math)
    # optionally add library and includes to main target
    # note that the EXTRA_LIBS variable is refactored away
    #   for multiple dependencies, it is better to use such variable to aggregate multiple libraries
    #   but in the current project there is only one library
    target_link_libraries(App PUBLIC Math)
    # removed usage of $EXTRA_INCLUDES:
    # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Math")
endif()

# removed usage of $EXTRA_LIBS
# removed $EXTRA_INCLUDES from
# target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})
target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}")