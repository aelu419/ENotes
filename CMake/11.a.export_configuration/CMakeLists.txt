# this section is in addition to section 7.a.installer
# it adds ability for the package to be used by other CMake projects

cmake_minimum_required(VERSION 3.15)
project(App VERSION 0.1.0)

add_library(app_compiler_flags INTERFACE)
target_compile_features(app_compiler_flags INTERFACE cxx_std_17)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(app_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

configure_file(Config.h.in Config.h)

add_subdirectory(Math)

add_executable(App app.cxx)
target_link_libraries(App PUBLIC Math)

target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}")

install(TARGETS App DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/Config.h" DESTINATION include)

# the exported MathTargets.cmake file (see Math/CMakeLists.txt) is installed here
install(EXPORT MathTargets FILE MathTargets.cmake DESTINATION lib/cmake/Math)


# note a new file has been added in the top level directory Config.cmake.in
# the file configures the package itself for discovery by CMake

# the command below is for including the Config
include(CMakePackageConfigHelpers)

# similar to a configure_file call but with several specifications for packages
# for example the @PACKAGE_INIT@ will be replaced with some relative paths
# this generates the MathConfig.cmake export
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/MathConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# writes another file to be used by CMake to specify package version
# the Math library's version will match the App's version or anything newer
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake"
  VERSION "${App_VERSION_MAJOR}.${App_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install both generated files to the Math library
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MathConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake
  DESTINATION lib/cmake/Math
)

# this allows the library to be used from the build directory itself without being installed first in
# the other CMake project that is referencing this project
export(EXPORT MathTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/MathTargets.cmake"
)

include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENCE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

set(CPACK_PACKAGE_VERSION_MAJOR "${App_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${App_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
