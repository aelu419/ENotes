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

# uses CPack to create platform specific installers
include(InstallRequiredSystemLibraries)

# include license
set(CPACK_RESOURCE_FILE_LICENCE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

# use the project target version (see top)
set(CPACK_PACKAGE_VERSION_MAJOR "${App_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${App_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

# to run CPack, go to the build binary directory and 
#   `cpack` to create package with default configs
#   `cpack -C Debug` to package with Debug flavor
#   `cpack -G ZIP` to use ZIP instead of TGZ
#   `cpack -C <config_name>` to use explicit configuration, like CPackSourceConfig.cmake in order to package the sourcecode

# note that under the MacOS environment at time of writing, an "App-0.1.0-Darwin.tar.gz" is created
#   the installer is a shell script named "App-0.1.0-Darwin.sh". when ran, it prompts the user for extraction and installation
# note that, like CMake itself, CPack also generates configuration files for future use
#   namely,  CPackConfig.cmake and CPackSourceConfig.cmake
