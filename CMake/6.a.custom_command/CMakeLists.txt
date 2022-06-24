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

install(TARGETS App DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/Config.h" DESTINATION include)

# enable_testing()

# function(do_test target arg result)
#     add_test(NAME Process${arg} COMMAND ${target} ${arg})
#     set_tests_properties(Process${arg} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
# endfunction()

# do_test(App 1 "1 \\+ 1 = 2")
# do_test(App 2 "1 \\+ 2 = 3")
# do_test(App 3 "1 \\+ 3 = 4")