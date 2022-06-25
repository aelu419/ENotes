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

include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENCE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

set(CPACK_PACKAGE_VERSION_MAJOR "${App_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${App_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

# this section replaces the native testing mentioned in 4.a.install_and_testing
include(CTest) # calls enable_testing() implicitly

# a CTestConfig.cmake file needs to be placed in the top level source directory
#   the file specifies several connections to the CDash server
#   usually, it is directly downloaded from CDash and not modified locally

# to test connected to CDash, run `ctest -VV`
#   CTest reads CTestConfig.cmake and operates with CDash