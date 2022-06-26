cmake_minimum_required(VERSION 3.15)
project(App VERSION 0.1.0)

# this section is to enable multiple configurations such as DEBUG and RELEASE
# by default, CMake assumes one build directory per configuration
# however it is possible to setup CPack with multiple configurations

set(CMAKE_DEBUG_POSTFIX d)

# to actually build the different configurations
#   enter the configuration's associated build directories (such as ./debug, ./release)
#   call `cmake -DCMAKE_BUILD_TYPE=<debug or release> ..
#   call `cmake --build .`

# to pack multiple configurations into one release, see MultiCPackConfig.cmake under the current source directory

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
# mark debug
set_target_properties(App PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(App PUBLIC Math)

target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}")

install(TARGETS App DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/Config.h" DESTINATION include)

install(EXPORT MathTargets FILE MathTargets.cmake DESTINATION lib/cmake/Math)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/MathConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake"
  VERSION "${App_VERSION_MAJOR}.${App_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MathConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake
  DESTINATION lib/cmake/Math
)

export(EXPORT MathTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/MathTargets.cmake"
)

include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENCE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

set(CPACK_PACKAGE_VERSION_MAJOR "${App_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${App_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

