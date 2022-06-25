cmake_minimum_required(VERSION 3.10)
project(App VERSION 0.1.0)

# option(USE_MATH "Use custom math implementation" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# # these only matter on windows (see the dll export section later)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# Shared libraries (SO file exported)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

configure_file(Config.h.in Config.h)

# always use Math
add_subdirectory(Math)

add_executable(App app.cxx)
target_link_libraries(App PUBLIC Math)

target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}")

install(TARGETS App DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/Config.h" DESTINATION include)

include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENCE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

set(CPACK_PACKAGE_VERSION_MAJOR "${App_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${App_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
