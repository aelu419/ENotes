# see Math/CMakeLists.txt for more changes

cmake_minimum_required(VERSION 3.10)
project(App VERSION 0.1.0)

option(USE_MATH "Use custom math implementation" ON)
configure_file(Config.h.in Config.h)

add_executable(App app.cxx)

if(USE_MATH)
    add_subdirectory(Math)
    target_link_libraries(App PUBLIC Math)
endif()

target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}")

# install executable target App
install(TARGETS App DESTINATION bin)
# install configured header
install(FILES "${PROJECT_BINARY_DIR}/Config.h" DESTINATION include)

# calling `cmake --install` runs the install commands
# installation location can be specified with the $CMAKE_INSTALL_PREFIX variable, or the `--prefix` flag
#   ex. run `cmake --install . --prefix "../install" under build

# add simple testing
enable_testing()

# function for testing
# note that these can also be called manually without this helper function
function(do_test target arg result)
    add_test(NAME Process${arg} COMMAND ${target} ${arg})
    set_tests_properties(Process${arg} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endfunction()

do_test(App 1 "1 \\+ 1 = 2") # declares a test called Process1 that adds 1 to 1 and checks if the output matches "1 + 1 = 2"
do_test(App 2 "1 \\+ 2 = 3")
do_test(App 3 "1 \\+ 3 = 4")

# in the build folder, run
#   `ctest -N` to count the tests
#   `ctest -VV` to run the tests